#!/usr/bin/python3
import os
import stat
import pickle
import pwd

data = {}
prev_data = {}

HOME = os.path.expanduser('~')
PICKLE_FILE = HOME + "/.audit_pickle"
clean_audit = True


# Save the data to a pickle
def save_data():
    save_data = open(PICKLE_FILE, "wb")
    pickle.dump(data, save_data)
    save_data.close()


# load the pickles
def load_data():
    data_f = open(PICKLE_FILE, "rb")
    loaded_data = pickle.load(data_f)
    data_f.close()
    return loaded_data


# check if a pickle exists for the data
def data_exists():
    return os.path.isfile(PICKLE_FILE)


def get_username(uid):
    return pwd.getpwuid(uid).pw_name


def audit_dir(dir_name):
    global data
    files = os.listdir(dir_name)
    for base_file in files:
        full_file = dir_name + "/" + base_file
        if (os.path.isfile(full_file)):
            stat = os.stat(full_file)
            data[full_file] = stat
        elif (os.path.isdir(full_file)):
            audit_dir(full_file)


def compare_data():
    global data, prev_data
    if not data_exists():
        # if the pickle doesn't exist that means we can't base
        # our audit on anything
        print("First time auditing, nothing to see here")
        save_data()
        return
    elif not data:
        print("Must check the directory before we audit anything...")
        return

    diff = list(set(data.keys() - set(prev_data.keys())))

    for file in diff:
        print("File %s was added" % file)

    for filename in prev_data:
        # previous stat of file
        if filename not in data:
            print("File %s was deleted" % filename)
            continue
        compare_file_stat(filename)

    save_data()


def compare_file_stat(filename):
    global clean_audit
    current_stat = data[filename]
    prev_stat = prev_data[filename]

    if current_stat.st_mode != prev_stat.st_mode:
        clean_audit = False
        print("Permissions for file %s were changed from %s to %s" %
              (filename, stat.filemode(prev_stat.st_mode),
               stat.filemode(current_stat.st_mode)))
    if current_stat.st_ino != prev_stat.st_ino:
        clean_audit = False
        print("Inode for file %s was changed from %s to %s" %
              (filename, prev_stat.st_ino, current_stat.st_ino))
    if current_stat.st_nlink != prev_stat.st_nlink:
        clean_audit = False
        print("Number of links for file %s was changed from %s to %s" %
              (filename, prev_stat.st_nlink, current_stat.st_nlink))
    if current_stat.st_uid != prev_stat.st_uid:
        clean_audit = False
        print("Owner for file %s was changed from %s to %s" %
              (filename, get_username(prev_stat.st_uid),
               get_username(current_stat.st_uid)))
    if current_stat.st_size != prev_stat.st_size:
        clean_audit = False
        print("Size of file %s was changed from %s to %s" %
              (filename, prev_stat.st_size, current_stat.st_size))
    if current_stat.st_mtime != prev_stat.st_mtime:
        clean_audit = False
        print("File %s was modified since last audit" % (filename))


if __name__ == "__main__":
    if data_exists():
        prev_data = load_data()
    audit_dir(HOME + "/workspace")
    compare_data()
    if clean_audit:
        print("Your workspace directory is squeaky clean")
