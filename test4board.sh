for i in {1..100}
do
echo "_1=TEST_BOT&_2=Test message #$i" | POST http://127.0.0.1/thread/3
done