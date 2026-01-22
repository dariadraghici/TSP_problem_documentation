#!/usr/bin/env python3
import os
import math
import random

def create_dirs():
	os.makedirs("input", exist_ok=True)
	os.makedirs("output_exact", exist_ok=True)
	os.makedirs("output_approx", exist_ok=True)

def write_test(test_id, n, edges):
	m = len(edges)
	with open(f"input/test{test_id:02d}.in", "w") as f:
		f.write(f"{n} {m}\n")
		for u, v, w in edges:
			f.write(f"{u} {v} {w}\n")

def generate_test00():
	write_test(0, -1, [])

def generate_test01():
	write_test(1, 1, [])

def generate_test02():
	edges = [(0, 1, 10)]
	write_test(2, 2, edges)

def generate_test03():
	edges = [(0,1,100), (1,2,100), (2,0,100)]
	write_test(3, 3, edges)

def generate_test04():
	edges = [
		(0,1,100), (1,2,100), (2,3,100), (3,0,100),
		(0,2,141), (1,3,141)
	]
	write_test(4, 4, edges)

def generate_test05():
	n = 7
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			w = abs(i-j) * 50
			edges.append((i, j, w))
	write_test(5, n, edges)

def generate_test06():
	n = 10
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			dist = min(abs(i-j), n-abs(i-j)) * 60
			edges.append((i, j, dist))
	write_test(6, n, edges)

def generate_test07():
	n = 8
	random.seed(42)
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			w = random.randint(10, 200)
			edges.append((i, j, w))
	write_test(7, n, edges)

def generate_test08():
	n = 12
	edges = []
	for i in range(n):
		xi, yi = i // 4, i % 4
		for j in range(i+1, n):
			xj, yj = j // 4, j % 4
			w = (abs(xi-xj) + abs(yi-yj)) * 80
			edges.append((i, j, w))
	write_test(8, n, edges)

def generate_test09():
	n = 10
	edges = []
	random.seed(42)
	for i in range(5):
		for j in range(i+1, 5):
			edges.append((i, j, random.randint(10, 50)))
	for i in range(5, 10):
		for j in range(i+1, 10):
			edges.append((i, j, random.randint(10, 50)))
	for i in range(5):
		for j in range(5, 10):
			edges.append((i, j, random.randint(200, 300)))
	write_test(9, n, edges)

def generate_test10():
	n = 10
	edges = []
	random.seed(43)
	for i in range(n):
		for j in range(i+1, n):
			base = abs(i-j) * 40
			noise = random.randint(-5, 5)
			edges.append((i, j, max(1, base + noise)))
	write_test(10, n, edges)

def generate_test11():
	n = 15
	edges = []
	random.seed(44)
	for i in range(n):
		for j in range(i+1, n):
			w = 500000 + random.randint(0, 500000)
			edges.append((i, j, w))
	write_test(11, n, edges)

def generate_test12():
	n = 20
	edges = []
	random.seed(45)
	for i in range(n):
		for j in range(i+1, n):
			w = 900000 + random.randint(0, 100000)
			edges.append((i, j, w))
	write_test(12, n, edges)

def generate_test13():
	n = 10
	edges = []
	random.seed(46)
	for i in range(n):
		for j in range(i+1, n):
			w = random.randint(1, 5)
			edges.append((i, j, w))
	write_test(13, n, edges)

def generate_test14():
	n = 15
	edges = []
	random.seed(47)
	for i in range(n):
		for j in range(i+1, n):
			if (i+j) % 3 == 0:
				w = random.randint(1, 10)
			elif (i+j) % 3 == 1:
				w = random.randint(100, 1000)
			else:
				w = random.randint(500000, 1000000)
			edges.append((i, j, w))
	write_test(14, n, edges)

def generate_test15():
	n = 8
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			w = 2 ** ((i+j) % 10 + 1)
			edges.append((i, j, w))
	write_test(15, n, edges)

def generate_test16():
	n = 12
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			w = (i*123456789 + j*987654321) % 1000000 + 1
			edges.append((i, j, w))
	write_test(16, n, edges)

def generate_test17():
	n = 15
	edges = []
	random.seed(48)
	for i in range(n):
		for j in range(i+1, n):
			w1 = random.randint(50, 150)
			edges.append((i, j, w1))
			w2 = random.randint(50, 150)
			edges.append((i, j, w2))
	write_test(17, n, edges)

def generate_test18():
	n = 20
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			base = abs(i-j)
			w = base * 30 + min(i,j) * 10
			edges.append((i, j, w))
	write_test(18, n, edges)

def generate_test19():
	n = 16
	edges = []
	for i in range(1, n):
		if i % 2 == 1:
			w = 30
		else:
			w = 100
		edges.append((0, i, w))
	
	for i in range(1, n):
		for j in range(i+1, n):
			w = 80
			edges.append((i, j, w))
	write_test(19, n, edges)

def generate_test20():
	n = 20
	edges = []
	random.seed(49)
	for i in range(n):
		for j in range(i+1, n):
			w = random.randint(1, 1000)
			edges.append((i, j, w))
	write_test(20, n, edges)

def generate_test21():
	n = 15
	edges = []
	for i in range(0, n, 2):
		if i+1 < n:
			edges.append((i, i+1, 10))
	edges.append((0, 5, 20))
	edges.append((5, 10, 20))
	write_test(21, n, edges)

def generate_test22():
	n = 20
	edges = []
	random.seed(50)
	for i in range(10):
		for j in range(i+1, 10):
			edges.append((i, j, random.randint(1, 20)))
	
	for i in range(10, 20):
		for j in range(i+1, 20):
			edges.append((i, j, random.randint(500, 1000)))
	
	for i in range(3):
		for j in range(3):
			edges.append((i, 10+j, random.randint(200, 300)))
	write_test(22, n, edges)

def generate_test23():
	n = 15
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			if i < 8 and j < 8:
				w = abs(i-j) * 40
			elif i >= 8 and j >= 8:
				w = abs(i-j) * 40
			else:
				w = 100
			edges.append((i, j, w))
	write_test(23, n, edges)

def generate_test24():
	n = 12
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			w = ((i*i + j*j) % 500) + 100
			edges.append((i, j, w))
	write_test(24, n, edges)

def generate_test25():
	n = 16
	edges = []
	for i in range(n):
		xi, yi = i // 4, i % 4
		for j in range(i+1, n):
			xj, yj = j // 4, j % 4
			if (xi+yi) % 2 == (xj+yj) % 2:
				w = 50
			else:
				w = 100
			edges.append((i, j, w))
	write_test(25, n, edges)

def generate_test26():
	n = 20
	edges = []
	for i in range(n):
		edges.append((i, (i+1)%n, 100))
	write_test(26, n, edges)

def generate_test27():
	n = 10
	edges = []
	for i in range(n):
		for j in range(i+1, n):
			edges.append((i, j, 1))
	write_test(27, n, edges)

def generate_test28():
	with open("input/test28.in", "w") as f:
		f.write("")

def generate_test29():
	with open("input/test29.in", "w") as f:
		f.write("3 3\n")
		f.write("0 1 100\n")
		f.write("1 2 abc\n")
		f.write("2 0 150\n")

def generate_test30():
	with open("input/test30.in", "w") as f:
		f.write("   \n")

def generate_test31():
	n = 10
	edges = []
	random.seed(51)
	for i in range(9):
		for j in range(i+1, 9):
			edges.append((i, j, random.randint(10, 30)))
	
	for i in range(9):
		edges.append((i, 9, 10000))
	write_test(31, n, edges)

def generate_test32():
	n = 16
	edges = []
	missing_pairs = [(0,5), (1,4), (10,15), (11,14)]
	
	for i in range(n):
		xi, yi = i // 4, i % 4
		for j in range(i+1, n):
			if (i,j) in missing_pairs or (j,i) in missing_pairs:
				continue
			
			xj, yj = j // 4, j % 4
			w = (abs(xi-xj) + abs(yi-yj)) * 60
			edges.append((i, j, w))
	write_test(32, n, edges)

def generate_test33():
	n = 15
	edges = []
	random.seed(52)
	def fractal_weight(i, j, level=3):
		if level == 0:
			return random.randint(10, 50)
		size = n // (2**level)
		if i // size == j // size:
			return fractal_weight(i%size, j%size, level-1)
		else:
			return 100 * level
	
	for i in range(n):
		for j in range(i+1, n):
			w = fractal_weight(i, j)
			edges.append((i, j, w))
	write_test(33, n, edges)

def generate_all():
	create_dirs()
	
	tests = [
		generate_test00, generate_test01, generate_test02, generate_test03,
		generate_test04, generate_test05, generate_test06, generate_test07,
		generate_test08, generate_test09, generate_test10, generate_test11,
		generate_test12, generate_test13, generate_test14, generate_test15,
		generate_test16, generate_test17, generate_test18, generate_test19,
		generate_test20, generate_test21, generate_test22, generate_test23,
		generate_test24, generate_test25, generate_test26, generate_test27,
		generate_test28, generate_test29, generate_test30, generate_test31,
		generate_test32, generate_test33
	]
	
	print(f"Generating {len(tests)} tests...")
	for i, test_func in enumerate(tests):
		print(f"  Test {i:02d}...")
		try:
			test_func()
		except Exception as e:
			print(f"    Error: {e}")
	
	print("\nDone! All 34 tests generated.")

if __name__ == "__main__":
	generate_all()