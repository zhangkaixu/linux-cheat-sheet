#!/bin/bash
cs_path=`pwd`
echo ${cs_path}
echo "这个脚本用来给新安装的Linux机器做一些自动配置工作"

link_file(){
    echo "链接 "$1
    rm ~/$1 -f
    ln -s ${cs_path}/$1 ~/$1
}

echo "处理vim配置文件 .vim/目录和.vimrc文件"
rm ~/.vim -r -f
ln -s ${cs_path}/_vim ~/.vim
rm ~/.vimrc -f
ln -s ${cs_path}/_vimrc ~/.vimrc

link_file .tmux.conf
link_file .bash_profile
link_file .screenrc
link_file .zshrc
echo "配置完成"
