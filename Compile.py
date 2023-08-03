# Compile the Gait Phase estimator, requires the intall of GCC. Tested on windows
import subprocess
import pathlib
import os

def handle_run(args, timeout=60, should_supress=False):
    try:
        subprocess.run(
            args, timeout=timeout, check=True, 
            stdout= subprocess.DEVNULL if should_supress else None
        )
    except FileNotFoundError as exc:
        print(f"Process with {args} failed because the executable could not be found.\n{exc}")
        quit()
    except subprocess.CalledProcessError as exc:
        print(
            f"Process with {args} failed because it did not return a successful return code. "
            f"Returned {exc.returncode}\n{exc}"
            )
        quit()
    except subprocess.TimeoutExpired as exc:
        print(f"Process with {args} time out.\n{exc}")
        quit()

def delete_old_build(rel_path):
    abs_path = rel_path+"\\build\\GetOutput.exe" # !WindowsGoo!
    if os.path.exists(abs_path):
        os.remove(abs_path)

def compile():
    # Get the current path
    path = pathlib.Path().resolve().__str__()
    # Delete the old build file if it exists
    delete_old_build(path)
    # Check for g++
    handle_run(["g++", "--version"], should_supress=True)
    # Compile
    abs_build_path = path+"\\build\\GetOutput" # !WindowsGoo!
    executable_ext = ".exe" # !WindowsGoo!
    handle_run(
        ["g++",
         "-std=c++20",
         path+"\src\*.cpp",
         path+"\GetOutput.cpp", 
         "-o",
          abs_build_path
          ], should_supress=True)
    return abs_build_path+executable_ext
    

if __name__ == "__main__":
    compile()