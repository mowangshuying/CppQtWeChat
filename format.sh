clang-format --version
find Client -name *.h -o -name *.cpp -exec unix2dos {} \;
find Client -name *.h -o -name *.cpp|xargs clang-format -i -style=file