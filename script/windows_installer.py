import os
import winreg


def get_user_environment_variable(name: str) -> str:
    try:
        with winreg.ConnectRegistry(None, winreg.HKEY_CURRENT_USER) as hkey:
            with winreg.OpenKey(hkey, r'Environment', 0, winreg.KEY_ALL_ACCESS) as sub_key:
                value, _ = winreg.QueryValueEx(sub_key, name)
    except:
        return ""
    
    return value

def set_user_environment_variable(name: str, value: str):
    with winreg.ConnectRegistry(None, winreg.HKEY_CURRENT_USER) as hkey:
        with winreg.OpenKey(hkey, r'Environment', 0, winreg.KEY_ALL_ACCESS) as sub_key:
            winreg.SetValueEx(sub_key, name, 0, winreg.REG_EXPAND_SZ, value)

if __name__ == '__main__':

    # Set three environment variables required for ccc runtime.
    if get_user_environment_variable('CCC_COMPILER') == '':
        set_user_environment_variable('CCC_COMPILER', 'g++')
    if get_user_environment_variable('CCC_INCLUDE_PATH') == '':
        set_user_environment_variable('CCC_INCLUDE_PATH', r'%USERPROFILE%\.ccc\inc')
    if get_user_environment_variable('CCC_LIBRARY_PATH') == '':
        set_user_environment_variable('CCC_LIBRARY_PATH', r'%USERPROFILE%\.ccc\lib')
    
    # Add the directory where the ccc executable program is located to the Path variable.
    Path_Value = get_user_environment_variable('Path')
    if r"%USERPROFILE%\.ccc\bin" not in Path_Value.split(';'):
        set_user_environment_variable('Path', r"%USERPROFILE%\.ccc\bin;" + Path_Value)
    Path_Value = get_user_environment_variable('Path')
    if r"%USERPROFILE%\.ccc\lib" not in Path_Value.split(';'):
        set_user_environment_variable('Path', r"%USERPROFILE%\.ccc\lib;" + Path_Value)

    pass