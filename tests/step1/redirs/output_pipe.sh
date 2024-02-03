# test valid no clobber

echo "set -o noclobber" >> ~/.bashrc

echo "echo hello > file" > a
echo "cat file" >| a

cat a

echo "set -o noclobber" >> ~/.bashrc

rm a