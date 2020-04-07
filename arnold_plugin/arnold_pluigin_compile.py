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
TARGET_FILE = "quantize.cpp"

for directory,_,sources in os.walk(DIR):
    for source in sources:
        if source.endswith(".cpp") and (not TARGET_FILE or source == TARGET_FILE):
            source_name,ext = os.path.splitext(source)
            source_path = os.path.join(directory,source)
            command = 'clang-cl /LD "{source_path}" /I "{ARNOLD_PATH}/include" "{ARNOLD_PATH}/lib/ai.lib" /link /out:"{source_name}.dll"'.format(source_name=source_name,source_path=source_path,ARNOLD_PATH=ARNOLD_PATH)
            subprocess.call(command,cwd=build_path,shell=True)
            
            exp_file = os.path.join(build_path,"%s.exp"%source_name)
            lib_file = os.path.join(build_path,"%s.lib"%source_name)
            os.remove(exp_file)
            os.remove(lib_file)

# import os
# import subprocess

# # maya_bin = r'C:\Program Files\Autodesk\Maya2017\bin'
# # command = r'''mayapy.exe -c "print 'clang-cl\n' " '''
# # print command
# # subprocess.Popen(command,cwd=maya_bin)

# maya_bin = r'D:/Maya 2019/Maya2019/bin'
# maya_bin = r'C:\Program Files\Autodesk\Maya2017\bin'
# command = r'''mayapy.exe -c "import maya.standalone;maya.standalone.initialize();import maya.cmds;maya.cmds.sphere();maya.cmds.file(rename='d:/testMayaPy.ma');maya.cmds.file(save=True, type='mayaAscii')"'''
# # subprocess.Popen(command, cwd=maya_bin)


# maya_bin = r'D:/Maya 2019/Maya2019/bin'
# maya_bin = r'"C:/Program Files/Autodesk/Maya2017/bin"'
# command = r'''cd {maya_bin} & mayapy.exe -c "import maya.standalone;maya.standalone.initialize();import maya.cmds;maya.cmds.sphere();maya.cmds.file(rename='d:/testMayaPy.ma');maya.cmds.file(save=True, type='mayaAscii')"'''.format(maya_bin=maya_bin)
# print(command)
# os.system(command.decode("utf-8"))


# import socket               # 导入 socket 模块
 
# s = socket.socket()         # 创建 socket 对象
 
# s.connect(('localhost', 7001))

# print s.send("print 1")
# s.close()