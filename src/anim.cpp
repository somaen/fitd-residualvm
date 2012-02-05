/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common.h"

namespace Fitd {

int initAnimInBody(int frame, char *anim, char *body) {
	int16 temp;
	int16 ax;
	int16 cx;
	int16 bx;
	char *saveAnim;
	int i;
	int flag;

	flag = (*(int16 *)body);

	temp = *(int16 *)anim;
	anim += 2;

	if(frame >= temp) {
		return(0);
	}

	ax = *(int16 *)anim;
	anim += 2;

	cx = ax;

	if(flag & 8) {
		ax = ((ax << 4) + 8) * frame;
	} else {
		ax = ((ax + 1) << 3) * frame;
	}

	anim += ax;

	animCurrentTime = *(int16 *)anim;
	animKeyframeLength = animCurrentTime;

	if(!(flag & 2)) {
		return(0);
	}

	body += 14;

	*(char **)(body + 2) = anim;
	*(uint16 *)(body + 6) = timer;

	body += *(int16 *)body;
	body += 2;

	ax = *(int16 *)body;
	bx = ax;

	body += (((ax << 1) + bx) << 1) + 2;

	bx = ax = *(int16 *)body;

	body += bx << 1;

	if(cx > ax)
		cx = ax;

	body += 10;

	saveAnim = anim;

	anim += 8;

	for(i = 0; i < cx; i++) {
		*(int16 *)(body) = *(int16 *)(anim);
		body += 2;
		anim += 2;
		*(int16 *)(body) = *(int16 *)(anim);
		body += 2;
		anim += 2;
		*(int16 *)(body) = *(int16 *)(anim);
		body += 2;
		anim += 2;
		*(int16 *)(body) = *(int16 *)(anim);
		body += 2;
		anim += 2;

		if(flag & 8) {
			*(int16 *)(body) = *(int16 *)(anim);
			body += 2;
			anim += 2;
			*(int16 *)(body) = *(int16 *)(anim);
			body += 2;
			anim += 2;
			*(int16 *)(body) = *(int16 *)(anim);
			body += 2;
			anim += 2;
			*(int16 *)(body) = *(int16 *)(anim);
			body += 2;
			anim += 2;
		}

		body += 8;
	}

	anim = saveAnim;

	anim += 2;

	animRot2 = *(int16 *)anim;
	anim += 2;
	animRot3 = *(int16 *)anim;
	anim += 2;
	animRot1 = *(int16 *)anim;
	anim += 2;

	return(1);

}

int anim(int animNum, int arg_2, int arg_4) {
	if(animNum == currentProcessedActorPtr->ANIM) {
		if(!(currentProcessedActorPtr->flags & 1)) {
			if(currentProcessedActorPtr->flags & 8) {
				deleteSub(currentProcessedActorIdx);
			}

			currentProcessedActorPtr->flags |= 1;

			initAnimInBody(currentProcessedActorPtr->FRAME, listAnim->get(animNum), listBody->get(currentProcessedActorPtr->bodyNum));

			currentProcessedActorPtr->field_40 = arg_2;
			currentProcessedActorPtr->field_42 = arg_4;
		} else {
			currentProcessedActorPtr->field_40 = arg_2;
			currentProcessedActorPtr->field_42 = arg_4;

			return(0);
		}
	} else {
		if(animNum == -1) {
			currentProcessedActorPtr->field_44 = -2;
			return(1);
		}

		if(!(currentProcessedActorPtr->flags & 1)) {
			if(currentProcessedActorPtr->flags & 8) {
				deleteSub(currentProcessedActorIdx);
			}

			currentProcessedActorPtr->flags |= 1;

			initAnimInBody(currentProcessedActorPtr->FRAME, listAnim->get(animNum), listBody->get(currentProcessedActorPtr->bodyNum));

			currentProcessedActorPtr->field_40 = arg_2;
			currentProcessedActorPtr->field_42 = arg_4;
		} else {
			if((currentProcessedActorPtr->field_40 & 2) || (currentProcessedActorPtr->field_46 & 2))
				return(0);

		}

		currentProcessedActorPtr->field_44 = animNum;
		currentProcessedActorPtr->field_46 = arg_2;
		currentProcessedActorPtr->field_48 = arg_4;
	}

	return(1);
}

void initBufferAnim(char *buffer, char *bodyPtr) {
	int flag = *(int16 *)bodyPtr;
	if(flag & 2) {
		char *source = bodyPtr + 0x10;
		int16 ax;
		int cx;
		int i;

		*(uint16 *)(source + 4) = (unsigned short int)timer;
		*(char **)(source) = buffer;

		source += *(int16 *)(source - 2);

		ax = *(int16 *)(source);

		ax = (((ax * 2) + ax) * 2) + 2;

		source += ax;

		cx = *(int16 *)source;

		source += cx * 2;

		buffer += 8;
		source += 10;

		for(i = 0; i < cx; i++) {
			*(int16 *)(buffer) = *(int16 *)(source);
			*(int16 *)(buffer + 2) = *(int16 *)(source + 2);
			*(int16 *)(buffer + 4) = *(int16 *)(source + 4);
			*(int16 *)(buffer + 6) = *(int16 *)(source + 6);

			buffer += 8;
			source += 8;

			if(flag & 8) {
				*(int16 *)(buffer) = *(int16 *)(source);
				*(int16 *)(buffer + 2) = *(int16 *)(source + 2);
				*(int16 *)(buffer + 4) = *(int16 *)(source + 4);
				*(int16 *)(buffer + 6) = *(int16 *)(source + 6);

				buffer += 8;
				source += 8;
			}

			source += 8;
		}

	}
}

int16 getAnimParam(char *animPtr) {
	return(*(int16 *)animPtr);
}

int16 getAnimType(char **bodyPtr) { // local
	int16 temp = *(int16 *)animVar1;

	animVar1 += 2;

	animVar4 += 2;

	*(int16 *)(*bodyPtr) = temp;
	(*bodyPtr) += 2;

	return(temp);
}

void processAnimRotation(char **bodyPtr, int bp, int bx) { // local
	int16 oldRotation = *(int16 *)animVar4;
	int16 newRotation;
	int16 diff;

	animVar4 += 2;

	newRotation = *(int16 *)animVar1;
	animVar1 += 2;

	diff = newRotation - oldRotation;

	if(diff == 0) {
		*(int16 *)(*bodyPtr) = newRotation;
	} else {
		if(diff <= 0x200) {
			if(diff >= -0x200) {
				*(int16 *)(*bodyPtr) = ((diff * bp) / bx) + oldRotation;
			} else {
				newRotation += 0x400;
				newRotation -= oldRotation;

				*(int16 *)(*bodyPtr) = ((newRotation * bp) / bx) + oldRotation;
			}
		} else {
			oldRotation += 0x400;
			newRotation -= oldRotation;

			*(int16 *)(*bodyPtr) = ((newRotation * bp) / bx) + oldRotation;
		}
	}

	(*bodyPtr) += 2;
}

void processAnimTranslation(char **bodyPtr, int bp, int bx) { // local
	int16 cx = *(int16 *)animVar4;
	int16 ax;
	animVar4 += 2;

	ax = *(int16 *)animVar1;
	animVar1 += 2;

	if(ax == cx) {
		*(int16 *)(*bodyPtr) = ax;
	} else {
		*(int16 *)(*bodyPtr) = (((ax - cx) * bp) / bx) + cx;
	}

	(*bodyPtr) += 2;
}

int16 setInterAnimObjet(int frame, char *animPtr, char *bodyPtr) {
	int numOfBonesInAnim = *(int16 *)(animPtr + 2);
	uint16 keyframeLength;
	uint16 timeOfKeyframeStart;
	char *animBufferPtr;
	int ax;
	uint16 bx;
	uint16 time;
	int bp;
	int flag;

	flag = *(int16 *)bodyPtr;

	animPtr += 4;

	if(flag & 8) {
		animPtr += ((numOfBonesInAnim << 4) + 8) * frame; // seek to keyframe
	} else {
		animPtr += ((numOfBonesInAnim + 1) * 8) * frame; // seek to keyframe
	}

	// animVar1 = ptr to the current keyFrame
	animVar1 = animPtr;

	keyframeLength = *(uint16 *)animPtr; // keyframe length

	if(!((*(int16 *)bodyPtr) & 2)) { // do not anim if the model can't be animated
		return(0);
	}

	bodyPtr += 16;

	animVar3 = bodyPtr;

	timeOfKeyframeStart = *(uint16 *)(bodyPtr + 4); // time of start of keyframe

	animBufferPtr = *(char **)(bodyPtr);

	if(!animBufferPtr) {
		animBufferPtr = animVar1;
	}

	// animVar4 = ptr to previous key frame
	animVar4 = animBufferPtr;

	bodyPtr += *(int16 *)(bodyPtr - 2);

	ax = *(int16 *)bodyPtr;
	ax = (ax * 6) + 2;
	bodyPtr += ax; // skip the points data

	ax = *(int16 *)bodyPtr; // num of bones
	bx = ax;
	bodyPtr += bx * 2; // skip bones idx table

	if(numOfBonesInAnim > ax) {
		numOfBonesInAnim = ax;
	}

	bodyPtr += 10; // skip bone 0

	time = (unsigned short int)timer - timeOfKeyframeStart;

	bx = keyframeLength;
	bp = time;

	if(time < keyframeLength) { // interpole keyframe
		char *animVar1Backup = animVar1;
		// skip bone 0 anim
		animVar4 += 8; // anim buffer
		animVar1 += 8; // current keyframe ptr

		if(!(flag & 8)) {
			do {
				switch(getAnimType(&bodyPtr)) {
				case 0: {
					processAnimRotation(&bodyPtr, bp, bx);
					processAnimRotation(&bodyPtr, bp, bx);
					processAnimRotation(&bodyPtr, bp, bx);
					break;
				}
				case 1:
				case 2: {
					processAnimTranslation(&bodyPtr, bp, bx);
					processAnimTranslation(&bodyPtr, bp, bx);
					processAnimTranslation(&bodyPtr, bp, bx);
					break;
				}
				}

				bodyPtr += 8;
			} while(--numOfBonesInAnim);
		} else {
			do {
				switch(getAnimType(&bodyPtr)) {
				case 0: {
					animVar4 += 6;
					animVar1 += 6;
					bodyPtr += 6;
					break;
				}
				case 1:
				case 2: {
					processAnimTranslation(&bodyPtr, bp, bx);
					processAnimTranslation(&bodyPtr, bp, bx);
					processAnimTranslation(&bodyPtr, bp, bx);
					break;
				}
				}

				processAnimRotation(&bodyPtr, bp, bx);
				processAnimRotation(&bodyPtr, bp, bx);
				processAnimRotation(&bodyPtr, bp, bx);

				animVar4 += 2;
				animVar1 += 2;
				bodyPtr += 10;

			} while(--numOfBonesInAnim);
		}

		animVar1 = animVar1Backup;

		animVar1 += 2;

		animRot2 = ((*(int16 *)(animVar1)) * bp) / bx; // X
		animRot3 = ((*(int16 *)(animVar1 + 2)) * bp) / bx; // Y
		animRot1 = ((*(int16 *)(animVar1 + 4)) * bp) / bx; // Z

		animVar1 += 6;

		animCurrentTime = bx;
		animKeyframeLength = bp;

		return(0);
	} else { // change keyframe
		char *tempBx = animVar1;
		char *si = animVar1;


		si += 8;

		do {
			*(int16 *)(bodyPtr) = *(int16 *)(si);
			*(int16 *)(bodyPtr + 2) = *(int16 *)(si + 2);
			*(int16 *)(bodyPtr + 4) = *(int16 *)(si + 4);
			*(int16 *)(bodyPtr + 6) = *(int16 *)(si + 6);

			bodyPtr += 8;
			si += 8;

			if(flag & 8) {
				*(int16 *)(bodyPtr) = *(int16 *)(si);
				*(int16 *)(bodyPtr + 2) = *(int16 *)(si + 2);
				*(int16 *)(bodyPtr + 4) = *(int16 *)(si + 4);
				*(int16 *)(bodyPtr + 6) = *(int16 *)(si + 6);
				bodyPtr += 8;
				si += 8;
			}

			bodyPtr += 8;

		} while(--numOfBonesInAnim);

		*(char **)animVar3 = animVar1;

		*(uint16 *)(animVar3 + 4) = (unsigned short int)timer;

		tempBx += 2;

		animCurrentTime = bx;
		animKeyframeLength = bx;

		animRot2 = *(int16 *)(tempBx);
		animRot3 = *(int16 *)(tempBx + 2);
		animRot1 = *(int16 *)(tempBx + 4);

		tempBx += 6;

		return(1);

	}

}

} // end of namespace Fitd
