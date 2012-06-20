#!/bin/bash
W=$0
cs_path=${W%/*}
cd
echo "这个程序用来给新安装的Linux机器做一些自动的配置"
echo "处理vim配置文件 .vim/目录和.vimrc文件"
rm .vim -r -f
ln -s ${cs_path}/_vim .vim
rm .vimrc
ln -s ${cs_path}/_vimrc .vimrc
echo "配置完成"
