import os
import sys
import glob   # Only for python 3.5+
import generate_txt_from_pydocs as gen_txt


''' Use Example:
python generate_txt_from_python_progs.py [SOURCE_FILE_DIR] [TARGET_DIRECTORY]
'''


def gen_txt_from_python_files(source_path, target_directory):
    os.system('mkdir ' + target_directory)
    target_directory = target_directory + \
        '/' if target_directory[-1] != '/' else target_directory

    f_target_mode = 'w'
    target_file_name = ''
    save_class_or_func_name_only = True # Setting this True overrides the save_entire_path
    save_entire_path = False
    for filename in glob.iglob(sys.argv[1] + '**/*.py', recursive=True):
        print(filename)
        if save_entire_path:
            # i.e. target_file_name = target_directory/main_module_sub_module1–sub_module2_filename
            target_file_name = target_directory + \
                filename.replace('/', '_').replace('.py', '')
        else:
            # i.e. target_file_name = filename.txt
            target_file_name = target_directory + \
                (filename.split('/')[-1]).replace('.py', '')

        with open(filename, 'r') as fr:
            split = []
            repeat = 0
            for line in fr:
                if repeat == 1:
                    target_file_name_with_module = target_file_name + '_' + \
                        (split[0])
                    # if we only want to save the func_name, this might cause collsions
                    if save_class_or_func_name_only:
                        target_file_name_with_module = target_directory + \
                            split[0]
                    target_file_name_with_module = target_file_name_with_module.replace(
                        'class ', '').replace('def ', '').replace('.', '_')
                    print("\tTARGET=", target_file_name_with_module)
                    with open(target_file_name_with_module+'.txt', f_target_mode) as fw:
                        fw.write('('.join(split))
                        for line in fr:
                            if line.find('def') == 0 or line.find('class') == 0:
                                break
                            else:
                                fw.write(line)
                if line.find('def') == 0 or line.find('class') == 0:
                    target_file_name_with_module = target_file_name + '_' + \
                        (line.split('(')[0])
                    # if we only want to save the func_name, this might cause collsions
                    if save_class_or_func_name_only:
                        target_file_name_with_module = target_directory + \
                            (line.split('(')[0])
                    target_file_name_with_module = target_file_name_with_module.replace(
                        'class ', '').replace('def ', '').replace('.', '_')
                    print("\tTARGET=", target_file_name_with_module)

                    with open(target_file_name_with_module+'.txt', f_target_mode) as fw:
                        fw.write(line)
                        for line in fr:
                            if line.find('def') == 0 or line.find('class') == 0:
                                repeat = 1
                                split = line.split('(')
                                break
                            else:
                                fw.write(line)


def main():
    if len(sys.argv) != 3:
        print(
            "Usage: python generate_txt_from_python_progs.py [SOURCE_FILE_DIR] [TARGET_DIRECTORY]")
        return
    gen_txt_from_python_files(sys.argv[1], sys.argv[2])


if __name__ == "__main__":
    main()
