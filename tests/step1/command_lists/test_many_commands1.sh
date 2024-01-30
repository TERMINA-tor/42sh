if true
then
	echo 'command1 = OK'
else
	echo 'hmmmmm'
fi

if false
then
	echo 'hmmmm'
else
	echo 'command2 = OK'
fi

if false
then
	echo 'hmmmmm'
elif false
then
	echo 'hmmmm ?'
else
	echo 'command3 = OK'
fi
