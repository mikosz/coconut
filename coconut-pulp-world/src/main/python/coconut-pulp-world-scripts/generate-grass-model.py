def frange(x, y, jump):
    while x < y - 0.000001:
        yield x
        x += jump

output = 'grass.obj'
blade_width = 0.006
x_blades = 1
x_step = 0.05 # from left edge to left edge
z_blades = 1
z_step = 0.05

preamble = '''mtllib grass.mtl
o grass

'''

textures = '''
vt 0.0 0.0
vt 1.0 0.0
vt 0.0 1.0
vt 1.0 1.0

'''

group_preamble = '''g blade
usemtl blade
s off

'''

x_start = -(float(x_blades) - 1.0) * x_step * 0.5 - (blade_width * 0.5 * (x_blades % 2))
x_end = x_start + x_blades * x_step
z_start = -(float(z_blades) - 1.0) * z_step * 0.5
z_end = z_start + z_blades * z_step

print("x_start = %f" % x_start)
print("x_end = %f" % x_end)
print("z_start = %f" % z_start)
print("z_end = %f" % z_end)

with open(output, 'w') as f:
    f.write(preamble)

    blades = 0

    for x in frange(x_start, x_end, x_step):
        for z in frange(z_start, z_end, z_step):
            blades += 1
            f.write("v %f 0.0 %f\n" % (x, z))
            f.write("v %f 0.0 %f\n" % (x + blade_width, z))
            f.write("v %f 0.4 %f\n" % (x, z))
            f.write("v %f 0.4 %f\n" % (x + blade_width, z))

    f.write(textures)
    f.write(group_preamble)

    for b in range(0, blades):
        v = b * 4 + 1
        f.write("f %d/1 %d/2 %d/3\n" % (v, v + 1, v + 2))
        f.write("f %d/3 %d/2 %d/4\n" % (v + 2, v + 1, v + 3))

print("Created a model with %d blades. Patch x_step = %f, z_step = %f" % (blades, x_step * x_blades, z_step * z_blades))
