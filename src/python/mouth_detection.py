from scipy.spatial import distance as dists
import dlib
import cv2
import numpy as np


def shape_to_np(shape_, dtype="int"):
	# initialize the list of (x, y)-coordinates
	coords = np.zeros((68, 2), dtype=dtype)

	# loop over the 68 facial landmarks and convert them
	# to a 2-tuple of (x, y)-coordinates
	for i in range(0, 68):
		coords[i] = (shape_.part(i).x, shape_.part(i).y)

	# return the list of (x, y)-coordinates
	return coords


def mouth_aspect_ratio(m):
	a = dists.euclidean(m[2], m[10])
	b = dists.euclidean(m[4], m[8])
	c = dists.euclidean(m[0], m[6])

	mar = (a + b) / (2.0 * c)
	return mar


class MouthDetection:
	def __init__(self):
		self.mouthStart = 48
		self.mouthEnd = 68
		self.detector = dlib.get_frontal_face_detector()
		self.predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')

	def __str__(self):
		return 'Mouth Detection Class'

	def get_mouth_aspect_ratio(self, img, debug_mouth=True, debug_face=True):
		gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
		rectangles = self.detector(gray, 0)

		for rect in rectangles:
			shape = self.predictor(gray, rect)
			shape = shape_to_np(shape)

			mouth = shape[self.mouthStart: self.mouthEnd]

			if debug_face:
				cv2.rectangle(img, (rect.left(), rect.top()), (rect.right(), rect.bottom()), (255, 0, 0), 2)

			if debug_mouth:
				mouth_hull = cv2.convexHull(mouth)
				cv2.drawContours(img, [mouth_hull], -1, (0, 255, 0), 1)

			mar_ = mouth_aspect_ratio(mouth)
			if mar_ is None:
				mar_ = 0.0
			return mar_

		return 0.0
