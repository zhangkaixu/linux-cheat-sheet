#!/bin/bash
cs_path=`pwd`
echo ${cs_path}
echo "这个脚本用来给新安装的Linux机器做一些自动配置工作"
echo "处理vim配置文件 .vim/目录和.vimrc文件"
rm ~/.vim -r -f
ln -s ${cs_path}/_vim ~/.vim
rm ~/.vimrc -f
ln -s ${cs_path}/_vimrc ~/.vimrc

rm ~/.tmux.conf -f
ln -s ${cs_path}/.tmux.conf ~/.tmux.conf


rm ~/.bash_profile -f
ln -s ${cs_path}/.bash_profile ~/.bash_profile


rm ~/.screenrc -f
ln -s ${cs_path}/.screenrc ~/.screenrc
rm ~/.zshrc -f
ln -s ${cs_path}/.zshrc ~/.zshrc
echo "配置完成"
