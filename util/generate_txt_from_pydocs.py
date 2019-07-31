import os
import sys

''' Use Example: python generate_funcs_from_modules [SOURCE_FILENAME] [TARGET_DIRECTORY]'''

def gen_txt_from_pydocs(filename, target_directory):
    os.system('mkdir '+target_directory)
    fmode = 'a' # filemode, set append to add function names with same name
    with open(filename, 'r') as readf:
        repeat = 0
        splitted = []
        for line in readf:
            if repeat == 1:
                func_name = ''
                if splitted[0].find('.') != -1:
                    dot_line = splitted[0].split('.')
                    func_name = dot_line[1]
                else:
                    func_name = splitted[0]
                func_name = func_name.replace('class ', '').replace('\n', '').replace('@', '')
                if target_directory[-1] != '/':
                    target_directory += '/'
                fname = target_directory+func_name+'.txt'
                print(fname)
                header_inc = 1
                header_inc = 0 if os.path.isfile(fname) else 1
                with open(fname, fmode) as writef:
                    if header_inc == 1:
                        writef.write(f'\"{func_name}\"\n')

                    writef.write('('.join(splitted))
                    for line in readf:
                        if (line[0].isspace()):
                            writef.write(line)
                        else:
                            break
            if not(line[0].isspace()):
                lines = line.split('(')
                func_name = ''
                if lines[0].find('.') != -1:
                    dot_line = lines[0].split('.')
                    func_name = dot_line[1]
                else:
                    func_name = lines[0]
                func_name = func_name.replace('class ', '').replace('\n', '').replace('@', '')
                if target_directory[-1] != '/':
                    target_directory += '/'
                fname = target_directory+func_name+'.txt'
                print(fname)
                header_inc = 1
                header_inc = 0 if os.path.isfile(fname) else 1
                with open(fname, fmode) as writef: # Append mode for repeat funcs
                    if header_inc == 1:
                        writef.write(f'\"{func_name}\"\n')

                    writef.write(line)
                    for line in readf:
                        if (line[0].isspace()):
                            writef.write(line)
                        else:
                            repeat = 1
                            splitted = line.split('(')
                            break

def main():
    if len(sys.argv) != 3:
        print("Usage: python generate_funcs_from_modules [SOURCE_FILENAME] [TARGET_DIRECTORY]")
        return
    gen_txt_from_pydocs(sys.argv[1], sys.argv[2])

if __name__ == "__main__":
    main()
