using TestImages, Images, Colors

infile = ARGS[1]
outfile = ARGS[2]

img = load(infile)
img = convert(Image{Gray}, img)

width, height = size(img)

newimg = zeros(img)

function median(x, y, k)
    values = Float32[]
    maxd = div(k, 2)
    for dx = -maxd:maxd
        for dy = -maxd:maxd
            if 1 <= x + dx <= width && 1 <= y + dy <= height
                push!(values, img[x + dx, y + dy])
            end
        end
    end
    sort!(values)
    median = values[div(length(values), 2) + 1]
    return median
end

for y = 1:height
    for x = 1:width
        simple = median(x, y, 5)
        newimg[x, y] = simple
    end
end


newimg = convert(Image{RGB}, newimg)
save(outfile, newimg)
