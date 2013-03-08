#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ISAM_H_
#define __ISAM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------- */

typedef struct _TNF_Root TNF_Root;
typedef struct _TNF_Branch TNF_Branch;
typedef struct _TNF_Leaf TNF_Leaf;

struct _TNF_Root {
	Branch* left;
	Branch* right;
	int idx;
} TNF_Root;

struct _TNF_Branch {
	Leaf* left;
	Leaf* right;
	int idx;
} TNF_Branch;

struct _TNF_Leaf {
	int idx;
} TNF_Leaf;

/* ----------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __ISAM_H_ */
