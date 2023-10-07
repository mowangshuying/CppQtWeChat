clang-format --version
find WeChatClient -name *.h -o -name *.cpp -exec unix2dos {} \;
find WeChatClient -name *.h -o -name *.cpp|xargs clang-format -i -style=file