import os

def get_user_environment_variable(name: str) -> str:
    try:
        with open(os.path.expanduser('~/.bashrc'), 'r') as file:
            for line in file:
                if line.startswith(f'export {name}='):
                    return line.split('=', 1)[1].strip().strip('"\'')
    except:
        return ""
    
    return ""

def set_user_environment_variable(name: str, value: str):
    with open(os.path.expanduser('~/.bashrc'), 'a') as file:
        file.write(f'export {name}="{value}"\n')

if __name__ == '__main__':
    # Set three environment variables required for ccc runtime.
    if get_user_environment_variable('CCC_COMPILER') == '':
        set_user_environment_variable('CCC_COMPILER', 'g++')
    
    # Add the directory where the ccc executable program is located to the Path variable.
    if get_user_environment_variable('PATH') != os.path.expanduser('~/.ccc/bin')+':$PATH':
        set_user_environment_variable('PATH', os.path.expanduser('~/.ccc/bin')+':$PATH')
    if get_user_environment_variable('LD_LIBRARY_PATH') != os.path.expanduser('~/.ccc/lib')+':$LD_LIBRARY_PATH':
        set_user_environment_variable('LD_LIBRARY_PATH', os.path.expanduser('~/.ccc/lib')+':$LD_LIBRARY_PATH')
    pass