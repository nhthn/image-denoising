using TestImages, Images, Colors

include("LpgPca.jl")
using LpgPca

image = load(ARGS[1])
image = convert(Image{Gray}, image)
image = convert(Array{Float64,2}, image)
const width, height = size(image)

denoised = copy(image)

L = 19
halfL = div(L, 2)
for y = (1 + halfL) : (height - halfL)
    println(y)
    for x = (1 + halfL) : (width - halfL)
        denoised[x, y] = denoise(image, x, y; L=L, σ=0.02)
    end
end

println("mse = ", mean((denoised - image) .^ 2))

save("before.png", convert(Image{Gray}, image))
save("after.png", convert(Image{Gray}, denoised))
