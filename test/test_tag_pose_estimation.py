#!/usr/bin/env python3

import sys
import os
import cv2
import numpy as np
from apriltag import apriltag

def parse_expected_detection(line):
    """
    Parse expected detection results from a line in the data file.
    Format: ID, (x1 y1), (x2 y2), (x3 y3), (x4 y4)
    """
    try:
        parts = line.strip().split(',')
        if len(parts) != 5:
            return None, None

        tag_id = int(parts[0].strip())
        
        corners = []
        for i in range(1, 5):
            corner_str = parts[i].strip().strip('()')
            x, y = map(float, corner_str.split())
            corners.append([x, y])
        
        return tag_id, np.array(corners)
    except (ValueError, IndexError):
        return None, None

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <image_path> <expected_data_path>")
        print(f"Example: {sys.argv[0]} data/33369213973_9d9bb4cc96_c.jpg data/33369213973_9d9bb4cc96_c.txt")
        return 1

    image_path = sys.argv[1]
    expected_data_path = sys.argv[2]

    print(f"Testing tag pose estimation with image: {image_path}")
    print(f"Expected data file: {expected_data_path}")

    # Load image
    image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    if image is None:
        print(f"Failed to load image: {image_path}")
        return 1

    # Create AprilTag detector - use tag36h11 family to match the C version
    detector = apriltag("tag36h11", 
                        threads=4,
                        decimate=1.0,  # Consistent with test_detection.c
                        refine_edges=False)  # Consistent with test_detection.c

    # Perform detection
    detections = detector.detect(image)

    # Read expected data file
    try:
        with open(expected_data_path, 'r') as expected_file:
            expected_lines = expected_file.readlines()
    except FileNotFoundError:
        print(f"Failed to open expected data file: {expected_data_path}")
        return 1

    print(f"Found {len(detections)} detections in image")

    # Perform pose estimation test for each detection
    num_detections = len(detections)
    pose_estimation_success = 0

    for i, det in enumerate(detections):
        print(f"Processing detection {i}: tag ID {det['id']}")

        # Set camera parameters and tag size
        tagsize = 0.16  # Assume tag size is 16cm
        fx = 600.0      # Assume focal length
        fy = 600.0
        cx = image.shape[1] / 2.0  # Assume optical center is at image center
        cy = image.shape[0] / 2.0

        # Perform pose estimation
        try:
            pose = detector.estimate_tag_pose(det, tagsize, fx, fy, cx, cy)
            R = pose['R']
            t = pose['t']
            err = pose['error']

            print(f"  Estimated pose for tag {det['id']}:")
            print(f"    Translation: [{t[0][0]:f}, {t[1][0]:f}, {t[2][0]:f}]")
            print(f"    Rotation matrix:")
            print(f"      [{R[0,0]:f}, {R[0,1]:f}, {R[0,2]:f}]")
            print(f"      [{R[1,0]:f}, {R[1,1]:f}, {R[1,2]:f}]")
            print(f"      [{R[2,0]:f}, {R[2,1]:f}, {R[2,2]:f}]")
            print(f"    Reprojection error: {err:f}")

            # Check if pose estimation was successful
            if err < 1.0:  # Assume error less than 1.0 means success
                pose_estimation_success += 1
        except Exception as e:
            print(f"  Pose estimation failed for tag {det['id']}: {e}")

    # Output test results
    print(f"\nPose estimation test results:")
    print(f"  Successful pose estimations: {pose_estimation_success}/{num_detections}")

    if pose_estimation_success == num_detections and num_detections > 0:
        print("  All pose estimations successful!")
    elif num_detections > 0:
        print("  Some pose estimations failed.")
    else:
        print("  No detections found in image.")

    return 0 if (pose_estimation_success == num_detections and num_detections > 0) else 1

if __name__ == "__main__":
    sys.exit(main())
