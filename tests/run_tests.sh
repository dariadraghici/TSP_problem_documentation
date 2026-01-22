#!/bin/bash

echo "Compiling programs..."
cd ../src
make clean && make
cd ../tests

mkdir -p output_exact output_approx

echo -e "\nRunning tests..."
last_test=33

# arrays pentru stocarea timpilor
declare -A exact_times
declare -A approx_times
declare -A speedups

for i in $(seq -f "%02g" 0 $last_test); do
	echo -e "\n================= Test $i ================="
	IN="input/test$i.in"
	OUT_E="output_exact/test$i.out"
	OUT_A="output_approx/test$i.out"

	[ ! -f "$IN" ] && echo "  Input file not found" && continue

	# execut solutia exacta cu masurarea timpului
	echo "Exact solution:"
	exact_time="0"
	
	if command -v time &> /dev/null; then
		# 'time' command daca este disponibil
		exec 3>&1 4>&2
		exact_time=$( { time -p ../src/TSP_exact < "$IN" > "$OUT_E" 2>&1; } 2>&1 | grep real | awk '{print $2}' )
		exec 3>&- 4>&-
		
		if [ $? -eq 0 ]; then
			exact_ran=1
			exact_times[$i]=$exact_time
			echo "  Output saved to $OUT_E"
		else
			echo "  Failed to run or timed out"
			exact_ran=0
			exact_times[$i]="N/A"
		fi
	else
		# Alternative pentru sistemele fara 'time' command
		start_time=$(date +%s.%N)
		if timeout 10s ../src/TSP_exact < "$IN" > "$OUT_E" 2>&1; then
			end_time=$(date +%s.%N)
			exact_time=$(echo "$end_time - $start_time" | bc -l)
			exact_ran=1
			exact_times[$i]=$exact_time
			echo "  Output saved to $OUT_E"
		else
			echo "  Failed to run or timed out"
			exact_ran=0
			exact_times[$i]="N/A"
		fi
	fi

	# execut solutia aproximativa cu masurarea timpului
	echo "Approximate solution:"
	approx_time="0"
	
	
	exec 3>&1 4>&2 # redirection pentru a captura output-ul comenzii time
	approx_time=$( { time -p ../src/TSP_approx < "$IN" > "$OUT_A" 2>&1; } 2>&1 | grep real | awk '{print $2}' ) # captura timpul real
	exec 3>&- 4>&-
	
	if [ $? -eq 0 ]; then # verific daca comanda a rulat cu succes
		approx_ran=1
		approx_times[$i]=$approx_time
		echo "  Output saved to $OUT_A"
	else
		echo "  Failed to run or timed out"
		approx_ran=0
		approx_times[$i]="N/A"
	fi
	

	# calcul speedup
	if [ $exact_ran -eq 1 ] && [ $approx_ran -eq 1 ] && [ "$exact_time" != "0" ] && [ "$approx_time" != "0" ]; then # daca ambele au rulat cu succes si timpii sunt validi
		if (( $(echo "$approx_time > 0" | bc -l 2>/dev/null) )) && (( $(echo "$exact_time > 0" | bc -l 2>/dev/null) )); then # evit impartirea la zero
			speedup=$(echo "scale=10; $exact_time / $approx_time" | bc -l 2>/dev/null || echo "N/A") # calcul speedup
			speedups[$i]=$speedup
		else
			speedups[$i]="N/A"
		fi
	else
		speedups[$i]="N/A"
	fi

	# comparare rezultate
	if [ $exact_ran -eq 1 ] && [ $approx_ran -eq 1 ]; then
		exact_val=$(head -n1 "$OUT_E")
		approx_val=$(head -n1 "$OUT_A")

		if [[ "$exact_val" == "Nu există soluție." ]]; then
			if [[ "$approx_val" == "Nu există soluție." ]]; then
				echo "  Both agree: Nu există soluție."
			else
				echo "  DISAGREEMENT: Exact says no solution, Approx found one"
			fi
		elif [[ "$approx_val" == "Nu există soluție." ]]; then
			echo "  DISAGREEMENT: Exact found solution, Approx says none"
		else
			if [ "$exact_val" = "$approx_val" ]; then
				echo "  Both found same optimal cost: $exact_val"
			else
				ratio=$(echo "scale=2; $approx_val / $exact_val" | bc -l 2>/dev/null)
				echo "  Exact: $exact_val, Approx: $approx_val (ratio: $ratio)"
			fi
		fi
	fi
done

echo -e "\n===================== Costs Comparison =====================\n"
echo "Test   Exact Cost           Approx Cost          Ratio"
echo "----   ----------           -----------          -----"

for i in $(seq -f "%02g" 0 $last_test); do
	exact_output="Missing"
	approx_output="Missing"
	ratio="N/A"

	if [ -f "output_exact/test$i.out" ]; then
		if grep -q "Nu există soluție." "output_exact/test$i.out"; then
			exact_output="Nu există soluție.  "
		else
			exact_output=$(head -n1 "output_exact/test$i.out")
		fi
	fi

	if [ -f "output_approx/test$i.out" ]; then
		if grep -q "Nu există soluție." "output_approx/test$i.out"; then
			approx_output="Nu există soluție.  "
		else
			approx_output=$(head -n1 "output_approx/test$i.out")
		fi
	fi

	if [[ "$exact_output" =~ ^[0-9]+$ ]] && [[ "$approx_output" =~ ^[0-9]+$ ]]; then
		if [ "$exact_output" -eq "$approx_output" ]; then
			ratio="1.00 (Match)"
		elif [ "$exact_output" -ne 0 ]; then
			ratio=$(echo "scale=2; $approx_output / $exact_output" | bc -l 2>/dev/null)
		fi
	elif [ "$exact_output" = "Nu există soluție.  " ] && [ "$approx_output" = "Nu există soluție.  " ]; then
		ratio="1.00 (Match)"
	elif [[ "$exact_output" != "Missing" && "$approx_output" != "Missing" && "$exact_output" != "$approx_output" ]]; then
		ratio="Different"
	fi

	printf "%-6s %-20s %-20s %-10s\n" "$i" "$exact_output" "$approx_output" "$ratio"
done

echo -e "\n=============== Execution Time Comparison ===============\n"
echo "Test   Exact Time           Approx Time           Speedup"
echo "----   ----------           -----------           -------"

trim_zeros() {
	echo "$1" | sed 's/\(\.\?0\+\)$//'
}

for i in $(seq -f "%02g" 0 $last_test); do
	exact_time_val=${exact_times[$i]:-"N/A"}
	approx_time_val=${approx_times[$i]:-"N/A"}
	speedup_val=${speedups[$i]:-"N/A"}

	if [[ "$exact_time_val" != "N/A" ]]; then
		exact_time_display=$(trim_zeros "$(printf "%.6f" "$exact_time_val" 2>/dev/null)")" s"
	else
		exact_time_display="N/A"
	fi

	if [[ "$approx_time_val" != "N/A" ]]; then
		approx_time_display=$(trim_zeros "$(printf "%.6f" "$approx_time_val" 2>/dev/null)")" s"
	else
		approx_time_display="N/A"
	fi

	if [[ "$speedup_val" != "N/A" ]] && [[ "$speedup_val" != "" ]]; then
		speedup_trimmed=$(trim_zeros "$(printf "%.10f" "$speedup_val" 2>/dev/null)")
		if [[ "$speedup_trimmed" == *"."* ]]; then
			int_part=${speedup_trimmed%.*}
			dec_part=${speedup_trimmed#*.}
			if [[ -z "$dec_part" || "$dec_part" =~ ^0+$ ]]; then
				speedup_display="${int_part}x"
			else
				clean_dec=$(echo "$dec_part" | sed 's/0\+$//')
				speedup_display="${int_part}.${clean_dec}x"
			fi
		else
			speedup_display="${speedup_trimmed}x"
		fi
	else
		speedup_display="N/A"
	fi

	printf "%-6s %-20s %-20s %-10s\n" "$i" "$exact_time_display" "$approx_time_display" "$speedup_display"
done

echo -e "\nNote: Speedup-ul este calculat ca Exact_Time / Approx_Time."