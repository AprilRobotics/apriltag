#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "apriltag.h"
#include "common/matd.h"

typedef struct {
    apriltag_detection_t* det;
    double tagsize; // In meters.
    double fx; // In pixels.
    double fy; // In pixels.
    double cx; // In pixels.
    double cy; // In pixels.
} apriltag_detection_info_t;

/**
 * This struct holds the transformation from the camera optical frame to
 * the April tag frame. The pose refers to the position of the tag within
 * the camera frame.
 */
typedef struct {
    matd_t* R; // Rotation matrix 3x3 of doubles.
    matd_t* t; // Translation matrix 3x1 of doubles.
} apriltag_pose_t;

/**
 * Estimate pose of the tag using the homography method described in [1].
 * @outparam pose
 */
void estimate_pose_for_tag_homography(
        apriltag_detection_info_t* info,
        apriltag_pose_t* pose);

/**
 * Estimate pose of the tag. This returns one or two possible poses for the
 * tag, along with the object-space error of each.
 *
 * This uses the homography method described in [1] for the initial estimate.
 * Then Orthogonal Iteration [2] is used to refine this estimate. Then [3] is
 * used to find a potential second local minima and Orthogonal Iteration is
 * used to refine this second estimate.
 *
 * [1]: E. Olson, "Apriltag: A robust and flexible visual fiducial system," in
 *      2011 IEEE International Conference on Robotics and Automation,
 *      May 2011, pp. 3400-3407.
 * [2]: Lu, G. D. Hager and E. Mjolsness, "Fast and globally convergent pose
 *      estimation from video images," in IEEE Transactions on Pattern Analysis
 *      and Machine Intelligence, vol. 22, no. 6, pp. 610-622, June 2000.
 *      doi: 10.1109/34.862199
 * [3]: Schweighofer and A. Pinz, "Robust Pose Estimation from a Planar Target,"
 *      in IEEE Transactions on Pattern Analysis and Machine Intelligence,
 *      vol. 28, no. 12, pp. 2024-2030, Dec. 2006.  doi: 10.1109/TPAMI.2006.252
 *
 * @outparam err1, pose1, err2, pose2
 */
void estimate_tag_pose_orthogonal_iteration(
        apriltag_detection_info_t* info,
        double* err1,
        apriltag_pose_t* pose1,
        double* err2,
        apriltag_pose_t* pose2,
        int nIters);

/**
 * Estimate tag pose.
 * This method is an easier to use interface to estimate_tag_pose_orthogonal_iteration.
 *
 * @outparam pose 
 * @return Object-space error of returned pose.
 */
double estimate_tag_pose(apriltag_detection_info_t* info, apriltag_pose_t* pose);

/**
 * Computes the second pose solution from a known first solution.
 *
 * Pose estimation from a planar target (such as an AprilTag) suffers from an
 * inherent ambiguity: two geometrically distinct poses can produce the same
 * image projection. This function attempts to recover this second candidate
 * solution using fix_pose_ambiguities(), then refines it via orthogonal iteration.
 *
 * If no second solution exists (unambiguous pose), solution2->R is left as NULL
 * and err2 is set to HUGE_VAL.
 *
 * @param[in]  v         Array of 4 normalized image points (3x1 column vectors),
 *                       expressed in normalized camera coordinates:
 *                       [(u - cx)/fx, (v - cy)/fy, 1]'.
 * @param[in]  p         Array of 4 object points in the tag frame (3x1 column vectors).
 * @param[in]  solution1 First pose solution (R and t already estimated and refined).
 *                       Used as a starting point to search for the second solution.
 * @param[out] solution2 Second candidate pose solution. If a second solution is found,
 *                       solution2->R and solution2->t are allocated by this function
 *                       and must be freed by the caller via matd_destroy().
 *                       Otherwise, solution2->R is NULL and solution2->t is undefined.
 * @param[in]  nIters    Number of iterations for refinement via orthogonal_iteration().
 * @param[out] err2      Object-space error associated with solution2 after refinement.
 *                       Set to HUGE_VAL if no second solution was found.
 *
 * @note The caller is responsible for freeing solution2->R and solution2->t
 *       (only if solution2->R != NULL) via matd_destroy().
 *
 * @see fix_pose_ambiguities()
 * @see orthogonal_iteration()
 * @see estimate_tag_pose_orthogonal_iteration()
 */
void get_second_solution(matd_t* v[4], matd_t* p[4], apriltag_pose_t* solution1, apriltag_pose_t* solution2, int nIters, double* err2);

#ifdef __cplusplus
}
#endif
