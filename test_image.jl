using TestImages, Images, Colors
include("LpgPca.jl")
using LpgPca

image = load(ARGS[1])
image = convert(Image{Gray}, image)
image = convert(Array{Float64,2}, image)
const width, height = size(image)

save("before.png", convert(Image{Gray}, image))

stage1 = copy(image)

L = 43
σ1 = 0.015

halfL = div(L, 2)
for y = (1 + halfL) : (height - halfL)
    println(y)
    for x = (1 + halfL) : (width - halfL)
        stage1[x, y] = denoise(image, x, y; L=L, σ=σ1)
    end
end

σ2 = 0.35 * sqrt(σ1 - mean((stage1 - image) .^ 2))
println("σ2 = ", σ2);

save("medial.png", convert(Image{Gray}, stage1))

stage2 = copy(stage1)

for y = (1 + halfL) : (height - halfL)
    println(y)
    for x = (1 + halfL) : (width - halfL)
        stage2[x, y] = denoise(stage1, x, y; L=L, σ=σ2)
    end
end

mse = (stage2 - image) .^ 2

save("after.png", convert(Image{Gray}, stage2))
