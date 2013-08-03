for ((  i = 1 ;  i <= 8;  i++  ))
do
  #tokenizer < testFiles/tokenizer-test.input.0$i > testFiles/tokenizer-test.output.0$i
  #parser < testFiles/parser-test.input.0$i > testFiles/parser-test.output.0$i
  #interpreter < testFiles/interpreter-test.input.0$i > testFiles/interpreter-test.output.0$i
  #interpreter < testFiles/interpreter2-test.input.0$i > testFiles/interpreter2-test.output.0$i
  #interpreter < testFiles/interpreter3-test.input.0$i > testFiles/interpreter3-test.output.0$i
done
