/*
 * JVM.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2014 - Giacomo Bergami
 *
 * socialsim is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * socialsim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with socialsim. If not, see <http://www.gnu.org/licenses/>.
 */

#include "JVM.h"
#include <iostream>

JavaVM* JVM::jvm = nullptr;

JavaVM* JVM::getInstance() {
	if (!jvm) {
		initJVM();
	}
	JNIEnv *lenv;
		
	int getEnvStat = jvm->GetEnv((void **)&lenv, JNI_VERSION_1_6);
	if (getEnvStat == JNI_EDETACHED) {
		std::cout << "GetEnv: not attached" << std::endl;
		if (jvm->AttachCurrentThread((void **) &lenv, NULL) != 0) {
		    std::cout << "Failed to attach" << std::endl;
		}
	} else if (getEnvStat == JNI_OK) {
		//
	} else if (getEnvStat == JNI_EVERSION) {
		std::cout << "GetEnv: version not supported" << std::endl;
	}
	
	return jvm;
}

void JVM::detach() {
	jvm->DetachCurrentThread();
}
