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

test('./test1', 0, '1')
test('./test2', 0, '123456789')
test('./test3', 1, 'Bad_Alloc')
