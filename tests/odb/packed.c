#include "clar_libgit2.h"
#include "odb.h"
#include "pack_data.h"

static git_odb *_odb;

void test_odb_packed__initialize(void)
{
    cl_git_pass(git_odb_open(&_odb, cl_fixture("testrepo.git/objects")));
}

void test_odb_packed__cleanup(void)
{
	git_odb_free(_odb);
	_odb = NULL;
}

void test_odb_packed__mass_read(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(packed_objects); ++i) {
		git_oid id;
		git_odb_object *obj;

		cl_git_pass(git_oid_fromstr(&id, packed_objects[i]));
		cl_assert(git_odb_exists(_odb, &id, 0) == 1);
		cl_git_pass(git_odb_read(&obj, _odb, &id));

		git_odb_object_free(obj);
	}
}

void test_odb_packed__read_header_0(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(packed_objects); ++i) {
		git_oid id;
		git_odb_object *obj;
		size_t len;
		git_otype type;

		cl_git_pass(git_oid_fromstr(&id, packed_objects[i]));

		cl_git_pass(git_odb_read(&obj, _odb, &id));
		cl_git_pass(git_odb_read_header(&len, &type, _odb, &id));

		cl_assert(obj->cached.size == len);
		cl_assert(obj->cached.type == type);

		git_odb_object_free(obj);
	}
}

void test_odb_packed__read_header_1(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(loose_objects); ++i) {
		git_oid id;
		git_odb_object *obj;
		size_t len;
		git_otype type;

		cl_git_pass(git_oid_fromstr(&id, loose_objects[i]));

		cl_assert(git_odb_exists(_odb, &id, 0) == 1);

		cl_git_pass(git_odb_read(&obj, _odb, &id));
		cl_git_pass(git_odb_read_header(&len, &type, _odb, &id));

		cl_assert(obj->cached.size == len);
		cl_assert(obj->cached.type == type);

		git_odb_object_free(obj);
	}
}

