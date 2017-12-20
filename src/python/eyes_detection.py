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


def eye_aspect_ratio(e):
	a = dists.euclidean(e[1], e[5])
	b = dists.euclidean(e[2], e[4])
	c = dists.euclidean(e[0], e[3])

	ear = (a + b) / (2.0 * c)

	return ear


class EyeDetection:
	def __init__(self):
		self.rightEyeStart = 36
		self.rightEyeEnd = 42
		self.leftEyeStart = 42
		self.leftEyeEnd = 48
		self.detector = dlib.get_frontal_face_detector()
		self.predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')

	def __str__(self):
		return 'Eyes Detection Class'

	def get_eyes_aspect_ratio(self, img, debug_eyes=True, debug_face=True):
		gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
		rectangles = self.detector(gray, 0)

		for rect in rectangles:
			shape = self.predictor(gray, rect)
			shape = shape_to_np(shape)

			left_eye = shape[self.leftEyeStart: self.leftEyeEnd]
			right_eye = shape[self.rightEyeStart: self.rightEyeEnd]

			if debug_face:
				cv2.rectangle(img, (rect.left(), rect.top()), (rect.right(), rect.bottom()), (255, 0, 0), 2)

			if debug_eyes:
				left_eye_hull = cv2.convexHull(left_eye)
				right_eye_hull = cv2.convexHull(right_eye)
				cv2.drawContours(img, [left_eye_hull], -1, (0, 255, 0), 1)
				cv2.drawContours(img, [right_eye_hull], -1, (0, 255, 0), 1)

			l_ear, r_ear = eye_aspect_ratio(left_eye), eye_aspect_ratio(right_eye)
			if l_ear is None:
				l_ear = 0.0
			if r_ear is None:
				r_ear = 0.0
			return l_ear, r_ear, True

		return 0.0, 0.0, False
