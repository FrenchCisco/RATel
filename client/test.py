var = "āĀēĒčČ..šŠūŪžŽ"

print(var)

var_encode = var.encode()
print(var_encode)

var_decode = var_encode.decode("utf8")
print(var_decode)


print("ddd".encode("utf8"))