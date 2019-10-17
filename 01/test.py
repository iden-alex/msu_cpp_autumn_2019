import subprocess
import time

def run(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = process.stdout.readlines()
    code = process.wait()
    return code, out

def test(command, expected_code, expected_value):
    print (command)
    code, out = run(command)
    if code != expected_code:
        print ('return value', expected_code, '(expected) !=', code)
        return
    line = out[0].decode("utf-8")
    try:
        if line.rstrip() != expected_value:
            print (expected_value, '(expected) !=', line.rstrip())
            return
    except ValueError:
        print ('invalid output')
        return
    except IndexError:
        print ('invalid output')
        return
    print ('ok')

test('./calc "2"', 0, '2')
test('./calc "-2"', 0, '-2')
test('./calc "2 + 2"', 0, '4')
test('./calc "2 + 2   "', 0, '4')
test('./calc "2/2/2"', 0, '0')
test('./calc "2 +- 2"', 0, '0')
test('./calc "   2+-4"', 0, '-2')
test('./calc "-    4- -4"', 0, '0')
test('./calc "2-3*4+-5/2"', 0, '-12')
test('./calc "2-3*4*2+1--2+-5/2"', 0, '-21')
test('./calc', 1, 'error')
test('./calc 2 + 3', 1, 'error')
test('./calc "2/0"', 1, 'error')
test('./calc "2/"', 1, 'error')
test('./calc "3 + a"', 1, 'error')
