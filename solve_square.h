#include "apriltag_pose.h"

/*
 * 	estimate tag pose using solve_square method.
 * 	vector (x,y,z) is the location of the center of the tag
 * 	vector (a,b,c) and (p,q,r) are vectors pointing from the center to the middle of two sides.
 *
 *             abc
 *     tag: ---------
 *          |   |   |       abc -> X
 *          |   ----|pqr    pqr -> Y 
 *          |  xyz  |       Z points inside
 *          ---------
 *
 *  solve_square method tries to solve x y z a b c p q r from 4 corners
 *  and then generate a rotation matrix from (a,b,c) and (p,q,r)
 */

double estimate_tag_pose_solve_square(apriltag_detection_info_t* info, apriltag_pose_t* pose);
