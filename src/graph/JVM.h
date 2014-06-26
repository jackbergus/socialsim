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

#include <jni.h>

#undef DEBUG

class JVM {

private:
	static JavaVM* jvm;
	
	static void initJVM() {

#ifndef NOJAVA
		JavaVMInitArgs args;
		JNIEnv* env;
		/* There is a new JNI_VERSION_1_4, but it doesn't add anything for the purposes of our example. */
		args.version = JNI_VERSION_1_4;
	
#ifdef DEBUG
		JavaVMOption options[3];
		args.nOptions = 3;
#else
		JavaVMOption options[2];
		args.nOptions = 2;
#endif
	
	

	
		options[0].optionString = const_cast<char*>("-Djava.class.path=.:graphview/:../ext-libs/graphstream/gs-algo-1.0.jar:../ext-libs/graphstream/gs-core-1.2.jar:../ext-libs/graphstream/gs-ui-1.2.jar");
		options[1].optionString = const_cast<char*>("-Xcheck:jni");
#ifdef DEBUG
		options[2].optionString = const_cast<char*>("-verbose");
#endif
		args.options = options;
		args.ignoreUnrecognized = JNI_FALSE;
	
		JNI_CreateJavaVM(&jvm, (void **)&env, &args);
#endif
	
	}

	JVM() { 
		initJVM();
	}
	
	
public:
	static JavaVM* getInstance();
	static void detach();
};
