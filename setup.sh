#!/bin/bash
cs_path=`pwd`
echo ${cs_path}
echo "这个脚本用来给新安装的Linux机器做一些自动配置工作"
echo "处理vim配置文件 .vim/目录和.vimrc文件"
rm ~/.vim -r -f
ln -s ${cs_path}/_vim ~/.vim
rm ~/.vimrc
ln -s ${cs_path}/_vimrc ~/.vimrc


ln -s ${cs_path}/.tmux.conf ~/.tmux.conf
echo "配置完成"
