package edu.berkeley.cs.sdb.SDBVision;

import android.graphics.ImageFormat;
import android.media.Image;
import android.media.ImageReader;
import android.os.Handler;
import android.view.Surface;

import java.nio.ByteBuffer;
import java.util.concurrent.atomic.AtomicBoolean;

public class AutoFitImageReader implements AutoCloseable {

    ImageReader mImageReader;
    OnImageAvailableListener mListener;
    AtomicBoolean mCaptureRequest;

    public interface OnImageAvailableListener {
        void onImageAvailable(byte[] imageData, int width, int height);
    }

    private final ImageReader.OnImageAvailableListener mOnImageAvailableListener = new ImageReader.OnImageAvailableListener() {
        @Override
        public void onImageAvailable(ImageReader reader) {
            Image image;
            try {
                image = reader.acquireLatestImage();
            } catch (IllegalStateException e) {
                e.printStackTrace();
                return;
            }

            if (image == null) {
                return;
            }

            // Stop transmitting images after one result comes back
            if (mCaptureRequest.getAndSet(false)) {
                // TODO: do the rotation and scaling here, this is a worker thread
                int width = image.getWidth();
                int height = image.getHeight();
                byte[] imageData = imageToBytes(image);
                image.close();

                if (!isBlurred(imageData, width, height)) {
                    return;
                }
                imageData = scale(imageData, width, height);
                imageData = rotate(imageData, width, height);
                mListener.onImageAvailable(imageData, width, height);
            } else {
                image.close();
            }
        }
    };

    public AutoFitImageReader(int width, int height, int format, int maxImages) {
        mImageReader = ImageReader.newInstance(width, height, format, maxImages);
        mCaptureRequest = new AtomicBoolean(false);
    }

    public Surface getSurface() {
        return mImageReader.getSurface();
    }

    public void close() {
        if (mImageReader != null) {
            mImageReader.close();
            mImageReader = null;
        }
    }

    public void setOnImageAvailableListener(OnImageAvailableListener listener, Handler handler) {
        mListener = listener;
        mImageReader.setOnImageAvailableListener(mOnImageAvailableListener, handler);
    }

    /**
     * Request the ImageReader to call the listener with the next imediate image.
     * This method is thread safe.
     */
    public void requestCapture() {
        mCaptureRequest.set(true);
    }

    /**
     * Takes an Android Image in the YUV_420_888 format and returns a byte array.
     * ref: http://stackoverflow.com/questions/30510928/convert-android-camera2-api-yuv-420-888-to-rgb
     *
     * @param image Image in the YUV_420_888 format
     * @return bytes that contains the image data in greyscale
     */
    private static byte[] imageToBytes(Image image) {
        if (image.getFormat() != ImageFormat.YUV_420_888) {
            return null;
        }

        int bytesPerPixel = ImageFormat.getBitsPerPixel(ImageFormat.YUV_420_888) / 8;
        Image.Plane yPlane = image.getPlanes()[0]; // we only need a gray picture
        int pixelStride = yPlane.getPixelStride();
        if (bytesPerPixel != 1 || pixelStride != 1) { // they are guaranteed to be both 1 in Y plane
            throw new RuntimeException("Wrong image format");
        }

        ByteBuffer buffer = yPlane.getBuffer();
        int width = image.getWidth();
        int height = image.getHeight();
        byte[] data = new byte[width * height];
        buffer.get(data);

        return data;
    }

    private static boolean isBlurred(byte[] image, int width, int height) {
        return false;
    }

    /**
     * Scale the image to have the same aspect ratio as the camera sensor
     *
     * @param imageData the raw bytes of a greyscale image, every byte is a color sample
     * @return the raw bytes of the scaled image, every byte is a color sample
     */
    private static byte[] scale(byte[] imageData, int width, int height) {
        // TODO read characteristics.get(CameraCharacteristics.SENSOR_INFO_ACTIVE_ARRAY_SIZE)
        return imageData;
    }

    /**
     * Rotate image to the user-inspecting orientation
     *
     * @param imageData the raw bytes of a greyscale image, every byte is a color sample
     * @return the raw bytes of the rotated image, every byte is a color sample
     */
    private static byte[] rotate(byte[] imageData, int width, int height) {
        // TODO read getResources().getConfiguration().orientation AND/OR getWindowManager().getDefaultDisplay().getRotation()
        return imageData;
    }

}