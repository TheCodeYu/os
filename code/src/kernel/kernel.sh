if ["$1" == "clean"];
then
make  clean -C $PWD
else
#创建lib bin等目录
mkdir -p $PWD/build
make -C $PWD
fi