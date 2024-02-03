echo toto > a

git log --oneline | grep wip >> a

cat a

rm a