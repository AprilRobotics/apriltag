from __future__ import print_function
import sysconfig
import re
conf = sysconfig.get_config_vars()

#for v in ('CC', 'CFLAGS', 'CCSHARED', 'INCLUDEPY', 'BLDSHARED', 'BLDLIBRARY', 'LDFLAGS', 'EXT_SUFFIX', 'MULTIARCH'):
    #print("{};{}".format(v, re.sub("[\t ]+", "\\ ", conf.get(v, ''))), end=';')
    #vals[v] = re.sub("[\t ]+", "\\ ", conf.get(v, ''))


print('CFLAGS', end=';')
c_flags = []
# Grab compiler flags minus the compiler itself.
c_flags.extend(conf.get('CC', '').split()[2:])
c_flags.extend(conf.get('CFLAGS', '').split())
c_flags.extend(conf.get('CCSHARED', '').split())
c_flags.append('-I{}'.format(conf.get('INCLUDEPY', '')))
c_flags.append('-Wno-strict-prototypes')
c_flags = [x for x in c_flags if not x.startswith('-O')]
print(' '.join(c_flags), end=';')

print('LDFLAGS', end=';')
print(conf.get('LDFLAGS', ''), end=';')

# Linker command.
print('LINKER', end=';')
print(conf.get('BLDSHARED', '') + conf.get('BLDLIBRARY', ''), end=';')

print('EXT_SUFFIX', end=';')
ext_suffix = '.so'
if conf.has_key('EXT_SUFFIX'):
    ext_suffix = conf['EXT_SUFFIX']
elif conf.has_key('MULTIARCH'):
    ext_suffix = conf['MULTIARCH'] + '.so'

print(ext_suffix, end=';')

# TODO: Strip out -g from 1 and 3. Strip out -O2 from 3.
