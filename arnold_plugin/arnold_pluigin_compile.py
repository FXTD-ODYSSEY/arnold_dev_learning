# coding:utf-8

__author__ =  'timmyliang'
__email__ =  '820472580@qq.com'
__date__ = '2020-04-07 12:54:24'

"""
auto compile the cpp file to the build folder
using LLVM clang-cl 
"""

import os
import subprocess

DIR = os.path.dirname(__file__)
build_path = os.path.join(DIR,"build")
if not os.path.exists(build_path):
    os.makedirs(build_path)

# NOTE arnold sdk | you can download it for different version - https://www.arnoldrenderer.com/arnold/download/archive/
ARNOLD_PATH = r"F:\Arnold\Arnold-5.2.2.0-windows"

for directory,_,sources in os.walk(DIR):
    for source in sources:
        if source.endswith(".cpp"):
            source_name,ext = os.path.splitext(source)
            source_path = os.path.join(directory,source)
            command = 'clang-cl /LD "{source_path}" /I "{ARNOLD_PATH}/include" "{ARNOLD_PATH}/lib/ai.lib" /link /out:"{source_name}.dll"'.format(source_name=source_name,source_path=source_path,ARNOLD_PATH=ARNOLD_PATH)
            subprocess.call(command,cwd=build_path,shell=True)
            
            exp_file = os.path.join(build_path,"%s.exp"%source_name)
            lib_file = os.path.join(build_path,"%s.lib"%source_name)
            os.remove(exp_file)
            os.remove(lib_file)

