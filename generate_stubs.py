#!/usr/bin/env python3
"""
Automatically generate Python stub file (.pyi) for the apriltag module.
This script extracts information from the C source code and docstrings.
"""

import os
import re
import sys


def extract_tag_families(source_file):
    """Extract supported tag families from the C source code."""
    families = []

    with open(source_file, 'r') as f:
        content = f.read()

    # Find the SUPPORTED_TAG_FAMILIES macro definition
    macro_pattern = r'#define\s+SUPPORTED_TAG_FAMILIES\([^)]+\)\s*\\?\s*((?:_\([^)]+\)\s*\\?\s*)+)'
    match = re.search(macro_pattern, content)

    if match:
        # Extract individual family names
        families_text = match.group(1)
        family_pattern = r'_\((\w+)\)'
        families = re.findall(family_pattern, families_text)

    return families


def extract_init_params(source_file):
    """Extract __init__ parameters from the C source code."""
    params = {
        'family': {'type': 'str', 'required': True},
        'threads': {'type': 'int', 'default': 1},
        'maxhamming': {'type': 'int', 'default': 1},
        'decimate': {'type': 'float', 'default': 2.0},
        'blur': {'type': 'float', 'default': 0.0},
        'refine_edges': {'type': 'bool', 'default': True},
        'debug': {'type': 'bool', 'default': False},
    }

    with open(source_file, 'r') as f:
        content = f.read()

    # Try to extract default values from the code
    # Look for variable declarations with default values
    patterns = {
        'Nthreads': r'int\s+Nthreads\s*=\s*(\d+)',
        'maxhamming': r'int\s+maxhamming\s*=\s*(\d+)',
        'decimate': r'float\s+decimate\s*=\s*([\d.]+)',
        'blur': r'float\s+blur\s*=\s*([\d.]+)',
        'refine_edges': r'bool\s+refine_edges\s*=\s*(true|false)',
        'debug': r'bool\s+debug\s*=\s*(true|false)',
    }

    for param_name, pattern in patterns.items():
        match = re.search(pattern, content)
        if match:
            value_str = match.group(1)
            if param_name == 'Nthreads':
                param_name = 'threads'

            if param_name in params:
                if params[param_name]['type'] == 'int':
                    params[param_name]['default'] = int(value_str)
                elif params[param_name]['type'] == 'float':
                    params[param_name]['default'] = float(value_str)
                elif params[param_name]['type'] == 'bool':
                    params[param_name]['default'] = (value_str == 'true')

    return params


def extract_docstring(docstring_file):
    """Extract docstring from the generated docstring header file."""
    if not os.path.exists(docstring_file):
        return None

    with open(docstring_file, 'r') as f:
        content = f.read()

    # The docstring is stored as a C string array with multiple lines
    # Each line is like: "text\n"
    # Extract all quoted strings after the = sign
    match = re.search(r'apriltag_\w+_docstring\[\]\s*=\s*\n((?:"[^"]*"\n)+)', content)
    if match:
        lines_block = match.group(1)
        # Extract all quoted strings
        quoted_strings = re.findall(r'"([^"]*)"', lines_block)
        # Join them together
        docstring = ''.join(quoted_strings)
        # Unescape special characters
        docstring = docstring.replace('\\n', '\n')
        docstring = docstring.replace('\\"', '"')
        docstring = docstring.replace('\\\\', '\\')
        return docstring.strip()

    return None


def generate_stub(source_dir, build_dir):
    """Generate stub content by analyzing source files."""

    source_file = os.path.join(source_dir, 'apriltag_pywrap.c')

    # Extract information from source
    tag_families = extract_tag_families(source_file)
    init_params = extract_init_params(source_file)

    # Try to extract docstrings from build directory
    detect_docstring_file = os.path.join(build_dir, 'apriltag_detect_docstring.h')
    type_docstring_file = os.path.join(build_dir, 'apriltag_py_type_docstring.h')

    detect_doc = extract_docstring(detect_docstring_file)
    type_doc = extract_docstring(type_docstring_file)

    # Generate TagFamily literal type
    if tag_families:
        families_str = ',\n    '.join(f'"{family}"' for family in tag_families)
        tag_family_type = f'TagFamily = Literal[\n    {families_str}\n]'
    else:
        # Fallback to hardcoded list
        tag_family_type = '''TagFamily = Literal[
    "tag36h10",
    "tag36h11",
    "tag25h9",
    "tag16h5",
    "tagCircle21h7",
    "tagCircle49h12",
    "tagStandard41h12",
    "tagStandard52h13",
    "tagCustom48h12"
]'''

    # Generate __init__ signature
    init_signature_parts = []
    for param_name, param_info in init_params.items():
        if param_name == 'family':
            init_signature_parts.append('family: TagFamily')
        else:
            type_name = param_info['type']
            default = param_info.get('default')
            if default is not None:
                if isinstance(default, bool):
                    default_str = str(default)
                elif isinstance(default, (int, float)):
                    default_str = str(default)
                else:
                    default_str = repr(default)
                init_signature_parts.append(f'{param_name}: {type_name} = {default_str}')
            else:
                init_signature_parts.append(f'{param_name}: {type_name}')

    init_signature = ',\n        '.join(init_signature_parts)

    # Use extracted docstring or fallback
    if type_doc:
        class_doc = f'"""{type_doc}"""'
    else:
        class_doc = '''"""AprilTag detector class.

    Create an AprilTag detector configured for a specific tag family.

    Args:
        family: Tag family name (e.g., "tag36h11", "tag25h9")
        threads: Number of threads to use (default: 1)
        maxhamming: Maximum Hamming distance for error correction (default: 1, max: 3)
        decimate: Detection resolution decimation factor (default: 2.0)
        blur: Gaussian blur sigma applied to image (default: 0.0)
        refine_edges: Whether to refine tag edges (default: True)
        debug: Enable debug mode (default: False)

    Example:
        >>> detector = apriltag.apriltag("tag36h11")
        >>> gray_image = np.array(...)  # uint8 grayscale image
        >>> detections = detector.detect(gray_image)
    """'''

    if detect_doc:
        detect_method_doc = f'"""{detect_doc}"""'
    else:
        detect_method_doc = '''"""Detect AprilTags in an image.

        Args:
            image: Grayscale uint8 image as a 2D numpy array with contiguous rows.

        Returns:
            Tuple of Detection dictionaries, one for each detected tag.
            Returns empty tuple if no tags are detected.

        Raises:
            RuntimeError: If image format is invalid or detection fails.

        Example:
            >>> detections = detector.detect(gray_image)
            >>> for det in detections:
            ...     print(f"Tag {det['id']} at {det['center']}")
        """'''

    stub_content = f'''\
"""AprilTags visual fiducial system detector.

This module provides Python bindings for the AprilTag detector library.

Auto-generated stub file for type hints.
Generated from source code analysis.
"""

from typing import Literal, TypedDict
import numpy as np
import numpy.typing as npt

__version__: str

class Detection(TypedDict):
    """Dictionary containing detection results for a single AprilTag."""
    hamming: int
    """Hamming distance between detection and true tag code."""
    margin: float
    """Decision margin - a measure of detection quality."""
    id: int
    """Numeric ID of the detected tag."""
    center: npt.NDArray[np.float64]
    """2-element array containing (x, y) coordinates of tag center."""
    lb_rb_rt_lt: npt.NDArray[np.float64]
    """4x2 array containing corner coordinates: left-bottom, right-bottom, right-top, left-top."""

{tag_family_type}

class apriltag:
    {class_doc}

    def __init__(
        self,
        {init_signature}
    ) -> None: ...

    def detect(self, image: npt.NDArray[np.uint8]) -> tuple[Detection, ...]:
        {detect_method_doc}
        ...
'''

    return stub_content


def main():
    if len(sys.argv) < 2:
        print("Usage: generate_stubs.py <output_file> [source_dir] [build_dir]")
        return 1

    output_path = sys.argv[1]

    # Determine source and build directories
    if len(sys.argv) >= 3:
        source_dir = sys.argv[2]
    else:
        # Assume script is in source directory
        source_dir = os.path.dirname(os.path.abspath(__file__))

    if len(sys.argv) >= 4:
        build_dir = sys.argv[3]
    else:
        # Assume build directory is a sibling of the output file
        build_dir = os.path.dirname(os.path.abspath(output_path))

    try:
        stub_content = generate_stub(source_dir, build_dir)

        # Write to file
        with open(output_path, 'w') as f:
            f.write(stub_content)

        print(f"Generated stub file: {output_path}")
        print(f"  Source directory: {source_dir}")
        print(f"  Build directory: {build_dir}")
        return 0

    except Exception as e:
        print(f"Error generating stub file: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
