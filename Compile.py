# Compile the Gait Phase estimator, requires the intall of GCC. Tested on windows
import subprocess
import pathlib

def handle_run(args, timeout=60, should_supress=False):
    try:
        subprocess.run(
            args, timeout=timeout, check=True, 
            stdout= subprocess.DEVNULL if should_supress else None
        )
    except FileNotFoundError as exc:
        print(f"Process with {args} failed because the executable could not be found.\n{exc}")
    except subprocess.CalledProcessError as exc:
        print(
            f"Process with {args} failed because it did not return a successful return code. "
            f"Returned {exc.returncode}\n{exc}"
            )
    except subprocess.TimeoutExpired as exc:
        print(f"Process with {args} time out.\n{exc}")

def compile():
    # Check for g++
    handle_run(["g++", "--version"], should_supress=True)
    # Get the current path
    path = pathlib.Path().resolve().__str__()
    # Compile
    handle_run(
        ["g++",
         path+"\src\*.cpp",
         path+"\GetOutput.cpp", 
         "-o",
          path+"\\build\\GetOutput" # !WindowsGoo!
          ])

if __name__ == "__main__":
    compile()