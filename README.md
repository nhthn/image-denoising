This project is an investigation into the topic of image denoising. We implemented two very different algorithms for removing noise from images:

- A smarter variant of median filtering that uses edge detection to prevent the blobby "oil painting" effect.
- A machine learning algorithm that identifies similar regions of the image and uses principal component analysis to separate noise from them.

## LPG-PCA ##

This implements Zhang et al., "Two-stage image denoising by principal component analysis with local pixel grouping" (published in *Pattern Recognition*).

We define the *target block* to be the 5x5 region centered on a pixel. We look for nearby 5x5 blocks that have a low mean squared error when compared to the target block, and select the best candidates. These candidates, along with the target block, are compiled into a training matrix **X**. **X** is transformed into the PCA domain, resulting in matrix **Y**. **Y** tends to be lopsided, with the noiseless features of the image concentrated in a few components. Thus noise can be removed from the image by zeroing out principal components identified as less important. After denoising, **Y** is transformed back into the space domain, from which the predicted value of the pixel can be extracted.

This process is repeated for every pixel on the image. Due to the considerable number of operations required on large marices, LPG-PCA is very slow. For this reason, we were forced to stick to monochrome.
