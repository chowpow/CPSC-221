// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"


// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const {
  int bdim = northwest->data.Dimension();
  PNG image(dimx * bdim, dimy * bdim);
  GridNode* curr = northwest;
  for (int y = 0; y < dimy; y++) {
    for (int x = 0 ; x < dimx; x++) {
      curr->data.Render(image, y*bdim, x*bdim);
      curr = curr->next;
    }
  }
  return image; 
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata) {
  GridNode* node = new GridNode(bdata);
  if (IsEmpty()) {
    southeast = node;
    northwest = node;
    node->prev = NULL;
    node->next = NULL;
  } else {
    southeast->next = node;
    node->next = NULL;
    node->prev = southeast;
    southeast = node;
  }
}

// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner)
{
	if (dimy != inner.dimy) {
		return;
	}
	GridNode* curr = northwest;
	GridNode* currother = inner.northwest;
	GridNode* temp;
   for (int y = 1; y <= dimy; y++) {
   	for (int x = 1; x <= dimx; x++) {
   		if (x == (dimx / 2)) {
   			temp = curr->next;
   			curr->next = currother;
   			currother->prev = curr;
   			for (int z = 1; z <= inner.dimx; z++) {
   				currother = currother->next;
   				curr = curr->next;
   			}
   			curr->next = temp;
   			curr->next->prev = curr;
   		}

   		curr = curr->next;

   	}
   }
   inner.northwest = NULL;
   inner.southeast = NULL;
   dimx = dimx + inner.dimx;
   inner.dimy = 0;
   inner.dimx = 0;
}

// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner)
{
	GridNode* curr = northwest;
	GridNode* temp;
  // even y dimension

	if (dimx != inner.dimx) {
		return;
	}
  for (int y = 1; y <= dimy; y++) {
    for (int x = 1; x <= dimx; x++) {
  	  if (y == (dimy/2) && x == dimx) {
  		  temp = curr->next;
  			curr->next = inner.northwest;
  			inner.northwest->prev = curr;
  			temp->prev = inner.southeast;
  			inner.southeast->next = temp;
  			inner.northwest = NULL;
  			inner.southeast = NULL;
				dimy = dimy + inner.dimy;
  			inner.dimx = 0;
  			inner.dimy = 0;
  			return;
  	  } 
      curr = curr->next;
  	}
  } 
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  GridNode* curr = northwest;
  GridNode* currOther = otherlist.northwest;
  GridNode* t;
  for (int y = 1; y <= dimy; y++) {
    for (int x = 1 ; x <= dimx; x++) {
        if (dimx % 2 == 0 && x == dimx) {
          curr = curr->next;
          currOther = currOther->next;    
        } else {
          if (y != dimy || x != dimx) {
          t = curr->next;
          curr->next = currOther->next;
          curr->next->prev = curr;
          currOther->next = t;
          currOther->next->prev = currOther;
          }
          curr = curr->next;
          currOther = currOther->next;
        }
      }
  }

  if ((dimx + dimy) % 2 == 1) {
    t = southeast;
    southeast = otherlist.southeast;
    otherlist.southeast = t;
  }
}

// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN()
{
  GridNode* curr = northwest;
  for (int y = 0; y < dimy; y++) {
    int a = (y % 2);
    if (a == 0) {
      for (int x = 1 ; x <= dimx; x++) {
        if (x % 2 == 0) {
        curr->data.Negative();
        }
        curr = curr->next;
      }
    } else {
      for (int x = 0 ; x < dimx; x++) {
        if (x % 2 == 0) {
        curr->data.Negative();
      }
      curr = curr->next;
      }
    }
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  if (IsEmpty()) {
    return;
  }
  if (northwest->next == NULL) {
    delete(northwest);
    return;
  }
  GridNode* ptr = northwest->next;
  GridNode* prev = northwest;
  while (!(ptr->next == NULL)) {
    delete(prev);
    prev = ptr;
    ptr = ptr-> next;
  }
  delete(ptr);
  delete(prev);
  northwest = NULL;
  southeast = NULL;
  dimx = 0;
  dimy = 0;
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{ 
  GridNode* currother = otherlist.northwest;
  GridNode* curr = northwest;
  while (curr && currother) {
    *curr = *currother;
     currother = currother->next;
     curr = curr->next;
  }
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//