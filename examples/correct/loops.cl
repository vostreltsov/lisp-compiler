(loop for i from -5 to 5 do
	(print i " "))
(print "\nfirst loop done\n")

(loop for i from 5 downto -5 do
	(print i " "))
(print "\nsecond loop done\n")

(loop for i from 1 to 1 do
	(print "this string should be repeated 1 time\n"))

(loop for i from 1 downto 1 do
	(print "this string also should be repeated 1 time\n"))

(loop for i from 1 to 0 do
	(print "you should not see this string\n"))

(loop for i from -1 downto 0 do
	(print "you also should not see this string\n"))

(loop for i from 1 to 3 do
	(loop for j from 1 to 3 do
		(print "a" i j " ")))

(print "\nwell done!\n")
