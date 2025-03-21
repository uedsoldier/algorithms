#!/usr/bin/env python3

import os
import platform
import shutil
import subprocess
import sys
from multiprocessing import cpu_count
from pathlib import Path

class TestBuilder:
    def __init__(self):
        # Find the repository root directory (where CMakeLists.txt is located)
        self.repo_root = self.find_repo_root()
        if not self.repo_root:
            print("Error: Could not find repository root with CMakeLists.txt")
            sys.exit(1)
            
        # Set build directory relative to the repository root
        self.build_dir = self.repo_root / 'build'
        self.platform = platform.system()
        self.cpu_count = cpu_count()
        
        # Change to the repository root directory
        os.chdir(self.repo_root)
        print(f"Working directory set to: {os.getcwd()}")

    def find_repo_root(self):
        """Find the repository root by looking for CMakeLists.txt"""
        # Start with the current directory
        current_dir = Path.cwd()
        
        # Also check the directory where the script is located
        script_dir = Path(__file__).parent.parent
        
        # Check current directory and its parents
        for directory in [current_dir, script_dir]:
            while directory != directory.parent:  # Stop at the root directory
                if (directory / 'CMakeLists.txt').exists():
                    return directory
                directory = directory.parent
                
        # If we get here, we couldn't find the repository root
        return None

    def cleanup_build(self):
        """Clean up the build directory if it exists"""
        if self.build_dir.exists():
            shutil.rmtree(self.build_dir, ignore_errors=True)
        self.build_dir.mkdir(exist_ok=True)

    def get_build_commands(self):
        """Get platform-specific build commands with debug flags"""
        if self.platform == 'Linux':
            return {
                'cmake': 'cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Debug',
                'make': f'make -j{self.cpu_count}'
            }
        elif self.platform == 'Windows':
            return {
                'cmake': 'cmake -S . -B build/ -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug',
                'make': f'mingw32-make -j{self.cpu_count}'
            }
        else:
            raise OSError(f'Unsupported OS/platform: {self.platform}')

    def run_command(self, command, cwd=None):
        """Run a command and handle errors"""
        try:
            print(f"Executing: {command}")
            result = subprocess.run(
                command,
                shell=True,
                check=True,
                cwd=cwd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            print(result.stdout)
            return True
        except subprocess.CalledProcessError as e:
            print(f"Error executing command: {command}")
            print(f"Error output: {e.stderr}")
            return False

    def build(self):
        """Run the build process"""
        try:
            print(f'Running on {self.platform} platform')
            
            # Cleanup build directory
            self.cleanup_build()
            
            # Get platform-specific commands
            commands = self.get_build_commands()
            
            # Run CMake
            if not self.run_command(commands['cmake']):
                return False
                
            # Run Make
            if not self.run_command(commands['make'], cwd=self.build_dir):
                return False
                
            print("Build completed successfully!")
            return True
            
        except Exception as e:
            print(f"Build failed: {str(e)}")
            return False

def main():
    builder = TestBuilder()
    success = builder.build()
    return 0 if success else 1

if __name__ == '__main__':
    exit(main())
