//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <thread_manage.h>

#define MIN(x, y) (((x) > (y)) ? (x) : (y))

// i = thread_number
// D = workspace
// w = thread_count
size_t calculate_work(size_t i, size_t D, size_t w) {
  return i * (D / w) + MIN(i, (D % w));
}

int create_shared_data(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // create memory
  pthread_mutex_init(&shared_data->can_access, NULL);
  // reserve memory for shared data
  // flag if password found
  shared_data->pass_found = (bool*)
    calloc(shared_data->count, sizeof(bool));
  // stores passwords for each file, conditionally safe
  shared_data->passwords = (char**)
    calloc(shared_data->count, sizeof(char*));
  if ((!shared_data->pass_found)
      || (!shared_data->passwords)
      || (!shared_data->max_char_pass)) {
    error = EXIT_FAILURE;
    return error;
  }
  for (size_t i = 0; i < shared_data->count; ++i) {
    shared_data->passwords[i] = (char*)
      calloc(shared_data->max_char_pass*2, sizeof(char));
    if (!shared_data->passwords[i]) {
      error = EXIT_FAILURE;
      return error;
    }
  }
  return error;
}

int create_threads(shared_data_t* shared_data, size_t file_num, size_t char_n) {
  assert(shared_data);
  int error = EXIT_SUCCESS;
  // reserve memory for threads
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  // reserve memory for private data
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    // assign memory and create memory
    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // sets id and file iteration
      private_data[thread_number].thread_id = thread_number;
      private_data[thread_number].file_num = file_num;
      private_data[thread_number].char_num = char_n;
      // initialize value
      private_data[thread_number].size_index_pass = 0;
      // assign public data
      private_data[thread_number].shared_data = shared_data;
      // memory for password
      private_data[thread_number].password = (char*)
        calloc(shared_data->max_char_pass *2, sizeof(char));
      private_data[thread_number].index_pass = (size_t*)
        calloc(100, sizeof(size_t));
      // memory for file (copy created)
      private_data[thread_number].file = (char*)
        calloc(100, sizeof(char));
      if ((!private_data[thread_number].password)
          || (!private_data[thread_number].index_pass)
          || (!private_data[thread_number].file)) {
        error = EXIT_FAILURE;
        return error;
      }
      // copy files
      copy_files(private_data, thread_number, file_num);

      // create_thread
      error = pthread_create(&threads[thread_number], NULL, found_pass
        , &private_data[thread_number]);
      if (error != EXIT_SUCCESS) {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = ERR_CREATE_THREAD;
        break;
      }
    }

    // free/destroy memory asked
    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);

      free(private_data[thread_number].index_pass);
      free(private_data[thread_number].password);
    }
    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      free(private_data[thread_number].file);
    }
    free(private_data);
    free(threads);
  }
  return error;
}

// inspired/taken from: https://acortar.link/i2pKg8
void rand_str(char *dest, size_t length) {
  char charset[] = "0123456789"
                   "abcdefghijklmnopqrstuvwxyz"
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  while (length-- > 0) {
    size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}

int copy_files(private_data_t* data, size_t th, size_t file_num) {
  shared_data_t* shared_data = data->shared_data;
  // create temporary directory
  int error = mkdir("./temp", 0777);
  if (error == EXIT_FAILURE) {
    return error;
  }

  FILE* original_file; FILE* copy_file;
  char* original_filename = shared_data->files[file_num];
  // random name for temp file
  char copy_filename[] = { [41] = '\1' };
  rand_str(copy_filename, sizeof copy_filename - 1);
  assert(copy_filename[41] == '\0');
  // put in directory wanted and .zip format
  copy_filename[0] = 't'; copy_filename[1] = 'e'; copy_filename[2] = 'm';
  copy_filename[3] = 'p'; copy_filename[4] = '/'; copy_filename[37] = '.';
  copy_filename[38] = 'z'; copy_filename[39] = 'i'; copy_filename[40] = 'p';

  char buffer[1024];
  size_t bytesRead;

  original_file = fopen(original_filename, "rb");
  if (original_file == NULL) {
    fprintf(stderr, "ERR_CANT_OPEN_FILE\n");
    return ERROR_FILE_OPEN;
  }

  copy_file = fopen(copy_filename, "wb");
  if (copy_file == NULL) {
    fprintf(stderr, "ERR_CANT_OPEN_FILE\n");
    fclose(original_file);
    return ERROR_FILE_OPEN;
  }

  // read original data and copy it
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), original_file)) > 0) {
    fwrite(buffer, 1, bytesRead, copy_file);
  }

  // assign temp file to thread;
  strcpy(data[th].file, copy_filename);

  // Close files
  fclose(original_file);
  fclose(copy_file);
  return EXIT_SUCCESS;
}

// inspired/taken from: https://rb.gy/a9y1r
int remove_directory(const char *path) {
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int r = -1;

  if (d) {
    struct dirent *p;

    r = 0;
    while (!r && (p=readdir(d))) {
      int r2 = -1;
      char *buf;
      size_t len;

      /* Skip the names "." and ".." as we don't want to recurse on them. */
      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        continue;

      len = path_len + strlen(p->d_name) + 2;
      buf = malloc(len);

      if (buf) {
        struct stat statbuf;

        snprintf(buf, len, "%s/%s", path, p->d_name);
        if (!stat(buf, &statbuf)) {
          if (S_ISDIR(statbuf.st_mode))
            r2 = remove_directory(buf);
          else
            r2 = unlink(buf);
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }

  if (!r)
    r = rmdir(path);

  return r;
}
