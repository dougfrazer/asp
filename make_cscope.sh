export DIR=`pwd`
cd /
find $DIR | grep ".*\.h$" > $DIR/cscope.files
find $DIR | grep ".*\.cpp$" >> $DIR/cscope.files
cd $DIR
cscope -b -q
