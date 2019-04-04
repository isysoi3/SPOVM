1. open cmd in folder with source code
2. type: vcvars32.bat (make sure you have environment variables configured)
3. type: cl win_process_child.cpp
4. type: cl win_process_parent.cpp
5. type: win_process_parent

Or with g++
1. open cmd in folder with source code
3. type: g++ win_process_child.cpp -o win_process_child
4. type: g++ win_process_parent.cpp -o win_process_parent
5. type: win_process_parent