BEGIN {
	ist_col = 4
	arg_col = 10
	cmt_col = 25
}

{
	line = $0

	# Split off comments
	cmt = ""
	if (match(line, /#.*/)) {
		cmt = substr(line, RSTART)
		line = substr(line, 1, RSTART - 1)
	}

	# Trim whitespace
	sub(/^[ \t]+/, "", line)
	sub(/[ \t]+$/, "", line)

	# Handle empty / comment lines
	if (line == "" && cmt != "") {
		printf "%*s%s\n", cmt_col, "", cmt
		next
	} else if (line == "" && cmt == "") {
		print ""
		next
	}

	# Detect label
	if (match(line, /^[A-Za-z_][A-Za-z0-9_]*:$/)) {
		printf "%s", line
		if (cmt != "") {
			printf "%*s%s", cmt_col - length(line), "", cmt
		}
		printf "\n"
		next
	}

	# Detect macro
	if (match(line, /^\.[A-Za-z_][A-Za-z0-9_ \\]*$/)) {
		printf "%s", line
		if (cmt != "") {
			printf "%*s%s", cmt_col - length(line), "", cmt
		}
		printf "\n"
		next
	}
	
	# Split instructions / arguments
	n = split(line, parts, /[ \t]+/)
	inst = parts[1]
	args = ""
	for (i = 2; i <= n; i++) {
		if (i > 2) args = args " "
		args = args parts[i]
	}

	# Print with alignment
	printf "%*s%-*s", ist_col, "", arg_col - ist_col, inst
	if (args != "") {
		printf "%s", args
	}

	if (cmt != "") {
		pad = cmt_col - (ist_col + length(inst) + (args == "" ? 0 : 1 + length(args)))
		if (pad < 1) pad = 1
		printf "%*s%s", pad, "", cmt
	}

	printf "\n"
}
