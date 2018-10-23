rm -rf ../inc
mkdir -p ../inc

cd ../libSrc 
src=`find ./ -name "*.h"`
dst=../inc
for one in $src 
do 
        #echo "cp header file $one"
        cp --parents $one $dst 
done

