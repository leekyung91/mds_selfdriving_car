echo '>>> producer start <<<'
./p &
sleep 5
echo '>>> consumer start <<<'
./c &
sleep 5
kill %2
kill %1
