# test a bad case of noclobber 

echo "set -o noclobber" >> ~/.bashrc

echo "echo hello > file" > a

cat a

echo "set +o noclobber" >> ~/.bashrc

rm a