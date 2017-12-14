from scipy.spatial import distance as dist
from imutils.video import VideoStream
from imutils import face_utils
import imutils
import time
import dlib
import cv2

MOUTH_AR_THRESH = 0.7
COUNTER = 0
MOUTH_AR_CONSEC_FRAMES = 4

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')

(mouthStart, mouthEnd) = face_utils.FACIAL_LANDMARKS_IDXS['mouth']

vs = VideoStream(0).start()
time.sleep(1.0)


def mouth_aspect_ratio(m):
	a = dist.euclidean(m[2], m[10])
	b = dist.euclidean(m[4], m[8])
	c = dist.euclidean(m[0], m[6])

	_mar = (a + b) / (2.0 * c)

	return _mar


while True:
	frame = vs.read()
	frame = imutils.resize(frame, width=450)
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

	rectangles = detector(gray, 0)

	for rect in rectangles:
		shape = predictor(gray, rect)
		shape = face_utils.shape_to_np(shape)

		mouth = shape[mouthStart:mouthEnd]

		mar = mouth_aspect_ratio(mouth)

		mouthHull = cv2.convexHull(mouth)
		cv2.drawContours(frame, [mouthHull], -1, (0, 255, 0), 1)

		if mar > MOUTH_AR_THRESH:
			COUNTER += 1

			if COUNTER >= MOUTH_AR_CONSEC_FRAMES:
				cv2.putText(frame, "DROWSINESS ALERT!", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

		else:
			COUNTER = 0

		cv2.putText(frame, "MAR: {:.2f}".format(mar), (300, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF

	if key == ord("q"):
		break

cv2.destroyAllWindows()
vs.stop()
