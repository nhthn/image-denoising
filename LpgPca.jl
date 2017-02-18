module LpgPca

# faster equivalent of diag(A * B)
mul_diagonal(A, B) = sum(A .* B', 2)

function denoise(
        image,
        x::Int,
        y::Int;
        K=5, # Size of analysis block
        L=19, # Size of training block
        σ=0.1 # Noise standard deviation
    )

    const halfK = div(K, 2)
    const halfL = div(L, 2)
    # Dimension of each block vector (= number of rows in the training matrix)
    const m = K^2

    # Number of columns in the training matrix
    const n = m * 8 + 1

    # Get the K*K analysis block centered around (tx,ty).
    getblock(tx, ty) = image[
        tx - halfK : tx + halfK,
        ty - halfK : ty + halfK
    ]

    # Target block, centered around (x,y).
    const target = getblock(x, y)

    # Mean squared error of a block relative to the target.
    mse(block) = mean((block - target) .^ 2)

    # Assemble a pool of blocks.
    blocks = Array{Float64,2}[]
    range = halfL - halfK
    for ty = y - range : y + range
        for tx = x - range : x + range
            # Exclude target
            if tx == x && ty == y
                continue
            end
            block = getblock(tx, ty)
            push!(blocks, block)
        end
    end
    # Sort so that the blocks of least MSE come first.
    sort!(blocks, by=mse, alg=PartialQuickSort(n - 1))

    # Construct the training matrix with the target and the best blocks reshaped into columns.
    Xυ = hcat(
        reshape(target, m),
        [reshape(block, m) for block in blocks[1:n - 1]]...
    )

    # Decenter the training matrix by subtracting the mean of each row.
    # We will add it back later.
    μ = mean(Xυ, 2)
    Xυ = Xυ .- μ

    # Noise covariance.
    Ωυ = σ^2 * eye(m)
    # Input covariance.
    ΩXυ = Xυ * Xυ' / n

    # PCA transformation matrix, formed by the eigenvector matrix of ΩXυ.
    # Since ΩXυ is symmetric, this matrix is real-valued.
    ΦX = eigfact(ΩXυ)[:vectors]
    PX = ΦX'

    # Transformed input
    Yυ = PX * Xυ

    # Covariance of transformed noise
    Ωυy = PX * Ωυ * PX'

    # Covariance of transformed input
    Ωyυ = Yυ * Yυ' / n

    # Covariance of transformed denoised output
    Ωy = max(Ωyυ - Ωυy, 0)

    # Shrinkage coefficients
    w = diag(Ωy) ./ (diag(Ωy) .+ diag(Ωyυ))

    # Transformed denoised output (we only need the first column)
    Y1 = Yυ[:,1] .* w

    # Denoised output (we only need the first column)
    X1 = PX' * Y1 .+ μ

    # The denoised target pixel is just the middle row
    X1[div(m,2) + 1]
end

export denoise

end
