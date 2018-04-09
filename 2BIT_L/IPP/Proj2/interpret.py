from __future__ import print_function
from argparse import ArgumentParser
import xml.etree.ElementTree as ET
import sys
import os
from enum import Enum



# TODO 1: Error msgs to stderr                               [solved]
# TODO 2: Check error codes, missing params, wrong params    []
# TODO 3: In symbolCheck calling varCheck may end program    [solved]
# TODO 4: Case-sensitive bool ?                              [solved]
# TODO 5: UTF-8 strings                                      [solved]
# TODO 6: Zasobnik ramcu, pri popu overit, zda jeste neex.   [solved]
# TODO 7: Strings ! (especially strings with only integers)  [solved]
# TODO 8: Empty string in XML -> argX.text -> error          [solved]
# TODO 9: Name and description in program element            [solved]
# TODO 10: String functions, checkString                     [solved]
# TODO 11: BOOL viz instr. READ specification                [solved]



# Function which prints multiple arguments to STDERR
def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

# Enumerate of avaible types in IPPcode18
class CVariableType(Enum):
    NONE   = -1
    INT    = 1
    BOOL   = 2
    STRING = 3

# Enumerate of avaible frames in IPPcode18
class CFrame(Enum):
    NONE = -1
    GF   = 1
    LF   = 2
    TF   = 3

# Class representing variable stored to the list in dictionary
class CVariable:
    m_init = False
    m_name = ''
    m_value = None
    m_type = CVariableType.NONE

    def __init__(self, name):
        self.m_name = name


# Class which saves data about interpret (variables; frames; data stack;) and performs analyzations + runs the interpret
class CInterpret:

    m_frames           = []     # List of frames
    no_frames          = 0      # Number of frames
    m_bTempFrameActive = False  # Is TF active ?
    m_tempFrame        = {}     # List of TF's
    m_labels           = {}     # List of labels
    m_PCs              = []     # List of return program counter values (return from CALL instruction)
    m_dataStack        = []     # Data stack -- PUSHS; POPS

    def __init__(self):
        self.no_frames = self.no_frames + 1;
        self.m_frames.append({})

	# Returns frame type (enum CFrame instantion)
    def getFrameType(self, frame):
        if frame == 'LF@':
            return CFrame.LF
        elif frame == 'GF@':
            return CFrame.GF
        elif frame == 'TF@':
            return CFrame.TF
        else:
            return CFrame.NONE

	# Returns variable type (enum CVariableType instantion)
    def getTypeCode(self, typeStr):
        if typeStr == 'int':
            return CVariableType.INT
        elif typeStr == 'string':
            return CVariableType.STRING
        elif typeStr == 'bool':
            return CVariableType.BOOL
        else:
            return CVariableType.NONE

	# Checks if frame/variable exists and is initialized, then returns its value or throws error
    def getVarValue(self, name, frame):
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)

        if frame == CFrame.GF:
            frame = 0
        elif frame == CFrame.LF:
            frame = self.no_frames-1
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)

            if not (name in self.m_tempFrame):
                eprint('ERROR -- Variable is not in frame.')
                sys.exit(54)

            if not self.m_tempFrame[name].m_init:
                eprint('ERROR -- Variable not inited.')
                sys.exit(56)

            return self.m_tempFrame[name].m_value

        if not (name in self.m_frames[frame]):
            eprint('ERROR -- Variable is not in frame.')
            sys.exit(54)

        if not self.m_frames[frame][name].m_init:
            eprint('ERROR -- Variable not inited.')
            sys.exit(56)

        return self.m_frames[frame][name].m_value

	# Checks if frame/variable exists and is initialized, then sets its value or throws error
    def setVarValue(self, name, frame, value):
        tmpFrame = frame
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)
        if frame == CFrame.GF:
            frame = 0
        elif frame == CFrame.LF:
            frame = self.no_frames-1
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)

            if not (name in self.m_tempFrame):
                eprint('ERROR -- Variable is not in frame.')
                sys.exit(54)

            self.m_tempFrame[name].m_init = True

            type2 = self.getVarType(name, tmpFrame)
            if type2 == CVariableType.STRING:
                self.m_tempFrame[name].m_value = self.stringCheck(value)
            else:
                self.m_tempFrame[name].m_value = value
            return

        if not (name in self.m_frames[frame]):
            eprint('ERROR -- Variable is not in frame.')
            sys.exit(54)

        self.m_frames[frame][name].m_init = True

        type2 = self.getVarType(name, tmpFrame)
        if type2 == CVariableType.STRING:
            self.m_frames[frame][name].m_value = self.stringCheck(value)
        else:
            self.m_frames[frame][name].m_value = value

	# Checks if frame/variable exists and is initialized, then sets its type or throws error
    def setVarType(self, name, frame, type):
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)
        if frame == CFrame.GF:
            frame = 0
        elif frame == CFrame.LF:
            frame = self.no_frames-1
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)

            if not (name in self.m_tempFrame):
                eprint('ERROR -- Variable is not in frame.')
                sys.exit(54)

            self.m_tempFrame[name].m_type = type
            return

        if not (name in self.m_frames[frame]):
            eprint('ERROR -- Variable is not in frame.')
            sys.exit(54)

        self.m_frames[frame][name].m_type = type


	# Checks if frame/variable exists and is initialized, then returns its type or throws error
    def getVarType(self, name, frame):
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)

        if frame == CFrame.GF:
            frame = 0
        elif frame == CFrame.LF:
            frame = self.no_frames-1
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)

            if not (name in self.m_tempFrame):
                eprint('ERROR -- Variable is not in frame.')
                sys.exit(54)

            if not self.m_tempFrame[name].m_init:
                eprint('ERROR -- Variable not inited.')
                sys.exit(56)

            return self.m_tempFrame[name].m_type

        if not (name in self.m_frames[frame]):
            eprint('ERROR -- Variable is not in frame.')
            sys.exit(54)

        if not self.m_frames[frame][name].m_init:
            eprint('ERROR -- Variable not inited.')
            sys.exit(56)

        return self.m_frames[frame][name].m_type

	# Checks if frame/variable exists, then returns its value or throws error
    def getVarTypeNoInit(self, name, frame):
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)
        if frame == CFrame.GF:
            frame = 0
        elif frame == CFrame.LF:
            frame = self.no_frames-1
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)
            if not (name in self.m_tempFrame):
                eprint('ERROR -- Variable is not in frame.')
                sys.exit(54)
            return self.m_tempFrame[name].m_type
        if not (name in self.m_frames[frame]):
            eprint('ERROR -- Variable is not in frame.')
            sys.exit(54)
        return self.m_frames[frame][name].m_type


    # === Frames ===

	# CREATEFRAME instruction, pushes TF to the top of stack
    def pushFrame(self):
        if self.m_bTempFrameActive == False:
            eprint('ERROR -- Undefined temporary frame.')
            sys.exit(55)

        self.no_frames = self.no_frames + 1;
        self.m_frames.append(self.m_tempFrame.copy())
        self.m_tempFrame.clear()
        self.m_bTempFrameActive = False

	# POPFRAME instruction, pops top of stack to the TF
    def popFrame(self):
        if self.no_frames == 1:
            eprint('ERROR -- Removing non-existing frame.')
            sys.exit(55)

        self.no_frames = self.no_frames - 1;
        self.m_tempFrame = self.m_frames.pop()
        self.m_bTempFrameActive = True



    # === Variables ===

	# Stores variable to the list in dictionary, DEFVAR instruction
    def addVar(self, name, frame):
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)

        if frame == CFrame.GF:
            self.m_frames[0][name] = CVariable(name)
        elif frame == CFrame.LF:
            self.m_frames[self.no_frames-1][name] = CVariable(name)
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)

            self.m_tempFrame[name] = CVariable(name)


	# MOVE instruction, inits the variable in list in dictionary, otherwise throws error if var doesnt exist in frame
    def initVar(self, name, value, type, frame):
        if frame == CFrame.LF and self.no_frames == 1:
            eprint('ERROR -- Local frame doesnt exist.')
            sys.exit(55)

        if frame == CFrame.GF:
            frame = 0
        elif frame == CFrame.LF:
            frame = self.no_frames-1
        else:
            if not self.m_bTempFrameActive:
                eprint('ERROR -- Temp frame doesnt exist.')
                sys.exit(55)

            if not (name in self.m_tempFrame):
                eprint('ERROR -- Variable doesnt exist.')
                sys.exit(54)

            self.m_tempFrame[name].m_init = True
            if type == CVariableType.STRING:
                self.m_tempFrame[name].m_value = self.stringCheck(value)
            else:
                self.m_tempFrame[name].m_value = value
            self.m_tempFrame[name].m_type = type
            return

        if not (name in self.m_frames[frame]):
            eprint('ERROR -- Variable doesnt exist.')
            sys.exit(54)

        self.m_frames[frame][name].m_init = True
        if type == CVariableType.STRING:
            self.m_frames[frame][name].m_value = self.stringCheck(value)
        else:
            self.m_frames[frame][name].m_value = value
        self.m_frames[frame][name].m_type = type


    # Syntax / Lex check functions

	# Checks syntax validity of variable - throws exception
    def varCheck(self, type, frameAtVar):
        ec = self.varCheckNoEx(type, frameAtVar)
        if ec != 0:
            sys.exit(ec)

	# Check syntax validity of variable - return error code
    def varCheckNoEx(self, type, frameAtVar):
        if type != 'var':
            eprint('ERROR -- Wrong type.')
            return 32
        frameAt = frameAtVar[:3]
        var = frameAtVar[3:]
        if not(frameAt == 'LF@' or frameAt == 'TF@' or frameAt == 'GF@'):
            eprint('ERROR -- Missing <frame> or/and <@>.')
            return 32
        if var[:1].isdigit():
            eprint('ERROR -- Wrong ID format.')
            return 32
        for c in var:
            if c.isspace() or not(c.isalnum() or c == '_' or c == '&' or c == '-' or c == '%' or c == '$' or c == '*'):
                eprint('ERROR -- Wrong ID format.')
                return 32
        return 0

	# Checks label validity and throws exception if its not
    def labelCheck(self, type, labelName):
        if type != 'label':
            eprint('ERROR -- Wrong type.')
            sys.exit(52)
        if labelName[:1].isdigit():
            eprint('ERROR -- Wrong Label format.')
            sys.exit(32)
        for c in labelName:
            if c.isspace() or not(c.isalnum() or c == '_' or c == '&' or c == '-' or c == '%' or c == '$' or c == '*'):
                eprint('ERROR -- Wrong Label format.')
                sys.exit(32)
        return True

	# Adds new label to the list of labels, if its already exists prints error + exit the interpret
    def addLabel(self, name, i):
        if name in self.m_labels:
            eprint('ERROR -- Label redefinition.')
            sys.exit(52)
        self.m_labels[name] = i

	# Checks symbol validity, may be variable or constant
    def symbCheck(self, type, value):
        testInt = True
        try:
            value = int(value)
        except ValueError:
            testInt = False

        if type == 'int' and testInt:
            return False
        elif type == 'bool' and (value == 'true' or value == 'false'):  # TODO: Case-sensitive bool ?
            return False
        elif type == 'string':
            self.stringCheck(value)
            return False
        elif self.varCheckNoEx(type, value) == 0:
            return True
        else:
            eprint('ERROR -- <symbol> expected.')
            sys.exit(32)

	# Checks type validity
    def typeCheck(self, type, valueType):
        if type != 'type':
            eprint('ERROR -- Missing frame.')
            sys.exit(52)
        if not(valueType == 'int' or valueType == 'bool' or valueType == 'string'):
            eprint('ERROR -- Wrong type.')
            sys.exit(52)
        return True

	# Checks string validity and also converts escape sequentions (\xyz) to the chracters
    def stringCheck(self, string):
        newStr = ''
        string = string.split()
        strLen = len(string)
        i = 0
        while i < strLen:
            c = string[i]
            if c == '#' or c.isspace():
                eprint('ERROR: Wrong string.')
                sys.exit(32)
            elif c == '\\':
                try:
                    c1 = string[i+1]
                    c2 = string[i+2]
                    c3 = string[i+3]
                    value = ((100 * int(c1)) + (10 * int(c2)) + int(c3))
                    if not(value >= 0 and value <= 999):
                        eprint('ERROR: Wrong string.')
                        sys.exit(32)
                except Exception:
                    eprint('ERROR: Wrong string.')
                    sys.exit(32)
                i += 3
                newStr += chr(value)
            else:
                newStr += c
            i += 1
        return newStr

# Takes a look which instruction it is, if its correct, then it checks its arguments validity and
# perform actions to execute this instruction
# Child -- XML atribute
# interpret -- Instantion of CInterpret class
# i -- i-th token
def decodeInstruction(child, interpret, i):
    if child.tag == 'instruction':

        # No arguments

        if child.attrib['opcode'] == 'CREATEFRAME' or child.attrib['opcode'] == 'POPFRAME' or \
                child.attrib['opcode'] == 'PUSHFRAME' or child.attrib['opcode'] == 'RETURN' or \
                child.attrib['opcode'] == 'BREAK':
            if len(child) != 0:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)

            if child.attrib['opcode'] == 'CREATEFRAME':
                interpret.m_bTempFrameActive = True
                interpret.m_tempFrame.clear()

            elif child.attrib['opcode'] == 'PUSHFRAME':
                interpret.pushFrame()

            elif child.attrib['opcode'] == 'POPFRAME':
                interpret.popFrame()

            elif child.attrib['opcode'] == 'RETURN':
                if len(interpret.m_PCs) == 0:
                    eprint('ERROR -- Return outside of function.')
                    sys.exit(56)
                return interpret.m_PCs.pop()

        # One argument <symb>

        elif child.attrib['opcode'] == 'PUSHS' or child.attrib['opcode'] == 'WRITE' or child.attrib[
            'opcode'] == 'DPRINT':
            if len(child) != 1:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            varConst = interpret.symbCheck(arg1.attrib['type'], arg1.text)
            if arg1.tag != 'arg1':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            if child.attrib['opcode'] == 'WRITE':
                if not varConst:
                    print(arg1.text)
                else:
                    val = interpret.getVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]))
                    print(val)

            elif child.attrib['opcode'] == 'PUSHS':
                if not varConst:
                    varType = interpret.getTypeCode(arg1.attrib['type'])
                    varValueStr = arg1.text
                else:
                    varType = interpret.getVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]))
                    varValueStr = interpret.getVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]))

                toCopy = CVariable('name')
                toCopy.m_type = varType
                toCopy.m_value = varValueStr
                interpret.m_dataStack.append(toCopy)

        # One argument <var>

        elif child.attrib['opcode'] == 'DEFVAR' or child.attrib['opcode'] == 'POPS':
            if len(child) != 1:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            interpret.varCheck(arg1.attrib['type'], arg1.text)
            if arg1.tag != 'arg1':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            if child.attrib['opcode'] == 'DEFVAR':
                interpret.addVar(arg1.text[3:], interpret.getFrameType(arg1.text[:3]))

            elif child.attrib['opcode'] == 'POPS':
                if len(interpret.m_dataStack) == 0:
                    eprint('ERROR -- Empty stack -- POPS.')
                    sys.exit(56)
                var = interpret.m_dataStack.pop()
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), var.m_value)
                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), var.m_type)


        # One argument <label>

        elif child.attrib['opcode'] == 'LABEL' or child.attrib['opcode'] == 'JUMP' or child.attrib['opcode'] == 'CALL':
            if len(child) != 1:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            interpret.labelCheck(arg1.attrib['type'], arg1.text)
            if arg1.tag != 'arg1':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            if child.attrib['opcode'] == 'JUMP':
                if not(arg1.text in interpret.m_labels):
                    eprint('ERROR -- Label doesnt exist.')
                    sys.exit(52)

                return interpret.m_labels[arg1.text]

            if child.attrib['opcode'] == 'CALL':
                if not(arg1.text in interpret.m_labels):
                    eprint('ERROR -- Label doesnt exist.')
                    sys.exit(52)

                interpret.m_PCs.append(i)
                return interpret.m_labels[arg1.text]


        # Two arguments <var> <type>

        elif child.attrib['opcode'] == 'READ':
            if len(child) != 2:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            interpret.varCheck(arg1.attrib['type'], arg1.text)
            arg2 = child.findall("*")[1]
            interpret.typeCheck(arg2.attrib['type'], arg2.text)
            if arg1.tag != 'arg1' or arg2.tag != 'arg2':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            readType = interpret.getTypeCode(arg2.text)

            value = None
            if readType == CVariableType.INT:
                try:
                    value = int(input())  # TODO: Shall it print smth ?
                except Exception:
                    value = 0
            elif readType == CVariableType.BOOL:
                value = input()
                if value == 'true':
                    value = 'true'
                else:
                    value = 'false'
            elif readType == CVariableType.STRING:
                value = str(input())

            interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), readType)
            interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), value)

        # Two arguments <var> <symb>

        elif child.attrib['opcode'] == 'MOVE' or child.attrib['opcode'] == 'INT2CHAR' or child.attrib[
            'opcode'] == 'STRLEN' or child.attrib['opcode'] == 'TYPE' or child.attrib['opcode'] == 'NOT':
            if len(child) != 2:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            interpret.varCheck(arg1.attrib['type'], arg1.text)
            arg2 = child.findall("*")[1]
            varConst = interpret.symbCheck(arg2.attrib['type'], arg2.text)
            if arg1.tag != 'arg1' or arg2.tag != 'arg2':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            if child.attrib['opcode'] == 'NOT':
                notValue = None
                if not varConst:
                    if arg2.attrib['type'] != 'bool':
                        eprint('ERROR: Type mismatch.')
                        sys.exit(53)
                    notValue = arg2.text
                else:
                    varType = interpret.getVarTypeNoInit(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                    if varType != CVariableType.BOOL:
                        eprint('ERROR: Type mismatch.')
                        sys.exit(53)
                    notValue = interpret.getVarValue(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))

                if notValue == 'true':
                    notValue = 'false'
                else:
                    notValue = 'true'

                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.BOOL)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), notValue)


            elif child.attrib['opcode'] == 'MOVE':
                if not varConst:
                    interpret.initVar(arg1.text[3:], arg2.text, interpret.getTypeCode(arg2.attrib['type']),
                                      interpret.getFrameType(arg1.text[:3]))
                else:
                    interpret.initVar(arg1.text[3:],
                                      interpret.getVarValue(arg2.text[3:], interpret.getFrameType(arg2.text[:3])),
                                      interpret.getVarType(arg2.text[3:], interpret.getFrameType(arg2.text[:3])),
                                      interpret.getFrameType(arg1.text[:3]))

            elif child.attrib['opcode'] == 'TYPE':
                if not varConst:
                    varTypeStr = arg2.attrib['type']
                else:
                    varType = interpret.getVarTypeNoInit(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                    if varType == CVariableType.INT:
                        varTypeStr = 'int'
                    elif varType == CVariableType.BOOL:
                        varTypeStr = 'bool'
                    elif varType == CVariableType.STRING:
                        varTypeStr = 'string'
                    else:
                        varTypeStr = ''

                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.STRING)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), varTypeStr)

            elif child.attrib['opcode'] == 'INT2CHAR':
                if not varConst:
                    if arg2.attrib['type'] != 'int':
                        eprint('ERROR: Type mismatch.')
                        sys.exit(53)
                    value = arg2.text
                else:
                    varType = interpret.getVarTypeNoInit(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                    if varType != CVariableType.INT:
                        eprint('ERROR: Type mismatch.')
                        sys.exit(53)
                    value = interpret.getVarValue(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))

                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.STRING)
                try:
                    interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), chr(int(value)))
                except ValueError:
                    eprint('ERROR -- Failed UTF-8')
                    sys.exit(58)

            elif child.attrib['opcode'] == 'STRLEN':
                if not varConst:
                    varType = interpret.getTypeCode(arg2.attrib['type'])
                    varValueStr = interpret.stringCheck(arg2.text)
                else:
                    varType = interpret.getVarType(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                    varValueStr = interpret.getVarValue(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                if varType != CVariableType.STRING:
                    eprint('ERROR: Type mismatch.')
                    sys.exit(53)
                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.INT)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), len(varValueStr))


        # Three arguments <label> <symb1> <symb2>

        elif child.attrib['opcode'] == 'JUMPIFEQ' or child.attrib['opcode'] == 'JUMPIFNEQ':
            if len(child) != 3:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            interpret.labelCheck(arg1.attrib['type'], arg1.text)
            arg2 = child.findall("*")[1]
            varConst1 = interpret.symbCheck(arg2.attrib['type'], arg2.text)
            arg3 = child.findall("*")[2]
            varConst2 = interpret.symbCheck(arg3.attrib['type'], arg3.text)
            if arg1.tag != 'arg1' or arg2.tag != 'arg2' or arg3.tag != 'arg3':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            if not varConst1:
                varType1     = interpret.getTypeCode(arg2.attrib['type'])
                varValueStr1 = arg2.text
            else:
                varType1     = interpret.getVarType(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                varValueStr1 = interpret.getVarValue(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))

            if not varConst2:
                varType2 = interpret.getTypeCode(arg3.attrib['type'])
                varValueStr2 = arg3.text
            else:
                varType2     = interpret.getVarType(arg3.text[3:], interpret.getFrameType(arg3.text[:3]))
                varValueStr2 = interpret.getVarValue(arg3.text[3:], interpret.getFrameType(arg3.text[:3]))

            if varType1 != varType2:
                eprint('ERROR -- Type mismatch.')
                sys.exit(53)

            if ((child.attrib['opcode'] == 'JUMPIFEQ' and varValueStr1 == varValueStr2) or \
                    (child.attrib['opcode'] == 'JUMPIFNEQ' and varValueStr1 != varValueStr2)):
                if not (arg1.text in interpret.m_labels):
                    eprint('ERROR -- Label doesnt exist.')
                    sys.exit(52)
                return interpret.m_labels[arg1.text]



        # Three arguments <var> <symb1> <symb2>

        elif child.attrib['opcode'] == 'ADD' or child.attrib['opcode'] == 'SUB' or child.attrib['opcode'] == 'MUL' or \
                child.attrib['opcode'] == 'IDIV' or child.attrib['opcode'] == 'LT' or child.attrib['opcode'] == 'GT' or \
                child.attrib['opcode'] == 'EQ' or child.attrib['opcode'] == 'AND' or child.attrib['opcode'] == 'OR' or \
                child.attrib['opcode'] == 'STRI2INT' or child.attrib['opcode'] == 'CONCAT' or \
                child.attrib['opcode'] == 'GETCHAR' or child.attrib['opcode'] == 'SETCHAR':
            if len(child) != 3:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = child.findall("*")[0]
            interpret.varCheck(arg1.attrib['type'], arg1.text)
            arg2 = child.findall("*")[1]
            varConst1 = interpret.symbCheck(arg2.attrib['type'], arg2.text)
            arg3 = child.findall("*")[2]
            varConst2 = interpret.symbCheck(arg3.attrib['type'], arg3.text)
            if arg1.tag != 'arg1' or arg2.tag != 'arg2' or arg3.tag != 'arg3':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)

            if not varConst1:
                varType1 = interpret.getTypeCode(arg2.attrib['type'])
                if varType1 == CVariableType.STRING:
                    varValueStr1 = interpret.stringCheck(arg2.text)
                else:
                    varValueStr1 = arg2.text
            else:
                varType1 = interpret.getVarType(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))
                varValueStr1 = interpret.getVarValue(arg2.text[3:], interpret.getFrameType(arg2.text[:3]))

            if not varConst2:
                varType2 = interpret.getTypeCode(arg3.attrib['type'])
                varValueStr2 = arg3.text
            else:
                varType2 = interpret.getVarType(arg3.text[3:], interpret.getFrameType(arg3.text[:3]))
                varValueStr2 = interpret.getVarValue(arg3.text[3:], interpret.getFrameType(arg3.text[:3]))

            if child.attrib['opcode'] == 'ADD' or child.attrib['opcode'] == 'SUB' or child.attrib['opcode'] == 'MUL' or \
                    child.attrib['opcode'] == 'IDIV':
                if varType1 != varType2:
                    eprint('ERROR -- Type mismatch.')
                    sys.exit(53)

                value = None
                if varType1 == CVariableType.INT:
                    if child.attrib['opcode'] == 'ADD':
                        value = int(varValueStr1) + int(varValueStr2)
                    elif  child.attrib['opcode'] == 'SUB':
                        value = int(varValueStr1) - int(varValueStr2)
                    elif  child.attrib['opcode'] == 'MUL':
                        value = int(varValueStr1) * int(varValueStr2)
                    elif  child.attrib['opcode'] == 'IDIV':
                        if varValueStr2 == '0':
                            eprint('ERROR -- Division by 0.')
                            sys.exit(57)
                        value = int(int(varValueStr1) / int(varValueStr2))

                    if value != None:
                        interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), value)
                        interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.INT)
                        value = None
                else:
                    eprint('ERROR -- Types mismatch.')
                    sys.exit(53)

            elif child.attrib['opcode'] == 'AND' or child.attrib['opcode'] == 'OR':
                if varType1 != varType2:
                    eprint('ERROR -- Type mismatch.')
                    sys.exit(53)

                value = None
                if varType1 == CVariableType.BOOL:
                    if child.attrib['opcode'] == 'AND':
                        if varValueStr1 == 'true' and varValueStr2 == 'true':
                            value = 'true'
                        else:
                            value = 'false'
                    elif child.attrib['opcode'] == 'OR':
                        if varValueStr1 == 'false' and varValueStr2 == 'false':
                            value = 'false'
                        else:
                            value = 'true'

                    if value != None:
                        interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), value)
                        interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.BOOL)
                        value = None
                else:
                    eprint('ERROR -- Types mismatch.')
                    sys.exit(53)

            elif child.attrib['opcode'] == 'LT' or child.attrib['opcode'] == 'GT' or child.attrib['opcode'] == 'EQ':
                if varType1 != varType2:
                    eprint('ERROR -- Type mismatch.')
                    sys.exit(53)

                value = None
                if varType1 == CVariableType.BOOL or varType1 == CVariableType.INT or varType1 == CVariableType.STRING:
                    if child.attrib['opcode'] == 'LT':
                        if varType1 == CVariableType.INT:
                            value = int(varValueStr1) < int(varValueStr2)
                        elif varType1 == CVariableType.BOOL:
                            if varValueStr1 == varValueStr2:
                                value = 'false'
                            elif varValueStr1 == 'true' and varValueStr2 == 'false':
                                value = 'false'
                            else:
                                value = 'true'
                        else:
                            value = varValueStr1 < varValueStr2

                    elif  child.attrib['opcode'] == 'GT':
                        if varType1 == CVariableType.INT:
                            value = int(varValueStr1) > int(varValueStr2)
                        elif varType1 == CVariableType.BOOL:
                            if varValueStr1 == varValueStr2:
                                value = 'false'
                            elif varValueStr1 == 'true' and varValueStr2 == 'false':
                                value = 'true'
                            else:
                                value = 'false'
                        else:
                            value = varValueStr1 > varValueStr2

                    elif  child.attrib['opcode'] == 'EQ':
                        if varType1 == CVariableType.INT:
                            value = int(varValueStr1) == int(varValueStr2)
                        elif varType1 == CVariableType.BOOL:
                            if varValueStr1 == varValueStr2:
                                value = 'true'
                            elif varValueStr1 == 'true' and varValueStr2 == 'false':
                                value = 'false'
                            else:
                                value = 'false'
                        else:
                            value = varValueStr1 == varValueStr2

                    if value == True:
                        value = 'true'
                    elif value == False:
                        value = 'false'

                    if value != None:
                        interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), value)
                        interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.BOOL)
                        value = None

                else:
                    eprint('ERROR -- Types mismatch.')
                    sys.exit(53)

            elif child.attrib['opcode'] == 'STRI2INT':
                if varType1 != CVariableType.STRING or varType2 != CVariableType.INT:
                    eprint('ERROR -- Types mismatch.')
                    sys.exit(53)
                if int(varValueStr2) >= len(varValueStr1):
                    eprint('ERROR -- Index outside of bounds.')
                    sys.exit(58)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), ord(varValueStr1[int(varValueStr2)]))
                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.INT)

            elif child.attrib['opcode'] == 'CONCAT':
                if varType1 != CVariableType.STRING or varType2 != CVariableType.STRING:
                    eprint('ERROR: Type mismatch.')
                    sys.exit(53)
                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.STRING)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), varValueStr1+varValueStr2)

            elif child.attrib['opcode'] == 'GETCHAR':
                if varType1 != CVariableType.STRING or varType2 != CVariableType.INT:
                    eprint('ERROR: Type mismatch.')
                    sys.exit(53)
                if int(varValueStr2) >= len(varValueStr1):
                    eprint('ERROR: Index outside of bounds.')
                    sys.exit(58)
                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.STRING)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), str(varValueStr1[int(varValueStr2)]))

            elif child.attrib['opcode'] == 'SETCHAR':
                t = interpret.getVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]))
                v = interpret.getVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]))
                if varType1 != CVariableType.INT or varType2 != CVariableType.STRING or t != CVariableType.STRING:
                    eprint('ERROR: Type mismatch.')
                    sys.exit(53)
                if len(varValueStr2) == 0 or (int(varValueStr1) >= len(v)):
                    eprint('ERROR: Index outside of bounds.')
                    sys.exit(58)
                v = v[:int(varValueStr1)] + varValueStr2[0] + v[int(varValueStr1)+1:]
                interpret.setVarType(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), CVariableType.STRING)
                interpret.setVarValue(arg1.text[3:], interpret.getFrameType(arg1.text[:3]), v)


        # Wrong instruction
        else:
            eprint('ERROR -- Unknown instruction')
            sys.exit(32)


    else:
        eprint('ERROR -- Instruction expected or/and wrong order of instructions.')
        sys.exit(32)

    return i


# Checks argument validity, parse arguments.
# Parse XML and checks its validity
# First saves all labels names
# Performs check actions on every instruction, executes them
def main():
    parser = ArgumentParser()
    #parser.add_argument("--help", help="Prints this help")
    parser.add_argument("--source", dest="myFile", help="Open specified file")

    try:
        args = parser.parse_args()
    except SystemExit:
        eprint('ERROR -- Wrong input params.')
        sys.exit(10)

    sourceFile = args.myFile

    # Empry file ? Pass
    if os.stat(sourceFile).st_size == 0:
        sys.exit(0)

    interpret = CInterpret()

    try:
        tree = ET.parse(sourceFile)
    except Exception:
        eprint('ERROR -- Wrong XML format.')
        sys.exit(31)

    root = tree.getroot()

    try:
        if root.tag != 'program':
            eprint('ERROR -- Root program is missing.')
            sys.exit(31)

        if len(root.attrib) > 3:
            eprint('ERROR -- Too many attributes.')
            sys.exit(31)

        if root.attrib['language'] != 'IPPcode18':
            eprint('ERROR -- Missing language header.')
            sys.exit(31)

        if len(root.attrib) == 3:
            if ('name' not in root.attrib) or ('description' not in root.attrib):
                eprint('ERROR -- Wrong attribute.')
                sys.exit(31)

        if len(root.attrib) == 2:
            if ('name' not in root.attrib) and ('description' not in root.attrib):
                eprint('ERROR -- Wrong attribute.')
                sys.exit(31)

    except Exception:
        eprint('ERROR -- Wrong header.')
        sys.exit(31)

    instrCount = len(root)


    # Save all label names and positions

    i = 0
    while i < instrCount:

        # Empty string workaround
        argCount = len(root[i])
        j = 0
        while j < argCount:
            arg = root[i].findall("*")[j]
            if arg.text is None:
                arg.text = ""
            j += 1

        if root[i].attrib['opcode'] == 'LABEL':
            if len(root[i]) != 1:
                eprint('ERROR -- XML -- Wrong count of arguments to instruction.')
                sys.exit(31)
            arg1 = root[i].findall("*")[0]
            interpret.labelCheck(arg1.attrib['type'], arg1.text)
            if arg1.tag != 'arg1':
                eprint('ERROR -- XML -- Wrong format.')
                sys.exit(31)
            interpret.addLabel(arg1.text, i)
        i += 1


    # Execute instructions

    i = 0
    while i < instrCount:
        try:
            i = decodeInstruction(root[i], interpret, i) + 1
        except Exception:
            eprint('ERROR -- Wrongly formated')
            sys.exit(31)

# Executes interpret
main()