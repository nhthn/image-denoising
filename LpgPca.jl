module LpgPca

function denoise(
        image,
        x::Int,
        y::Int;
        K=5, # Size of analysis block
        L=19, # Size of training block
        σ=0.15 # Noise standard deviation
    )

    const halfK = div(K, 2)
    const halfL = div(L, 2)
    # Dimension of each block vector (= number of rows in the training matrix)
    const m = K^2

    # Get the K*K analysis block centered around (tx,ty).
    getblock(tx, ty) = reshape(image[
        tx - halfK : tx + halfK,
        ty - halfK : ty + halfK
    ], m)

    # Target block, centered around (x,y).
    const target = getblock(x, y)

    # Mean squared error of a block relative to the target.
    mse(block) = mean((block - target) .^ 2)

    # Assemble a pool of blocks.
    blocks = Vector[]
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
    sort!(blocks, by=mse)

    # Construct the training matrix with the target and the best blocks.
    Xυ = hcat(target, blocks[1:m * 8]...)
    n = m * 8 + 1

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
    #Ωy = Ωyυ - Ωυy

    # Shrinkage coefficients
    w = diag(Ωy) ./ (diag(Ωy) .+ diag(Ωyυ))

    # Transformed denoised output
    Y = Yυ .* w

    # Denoised output
    X = PX' * Y .+ μ

    X[div(m,2) + 1, 1]
end

export denoise

end
