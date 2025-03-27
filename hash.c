#include <stddef.h>
#include <stdio.h>

typedef struct {
  char *key;
  void *value;
} HashNode;

typedef struct {
  HashNode *values;
  size_t capacity;
  size_t size;
} HashTable;

int hash_function(char *key, size_t size);

int main() {
  int is[166] = {
hash_function("a", 9000), 
hash_function("ac", 9000), 
hash_function("accumsan", 9000), 
hash_function("adipiscing", 9000), 
hash_function("Aenean", 9000), 
hash_function("aliquam", 9000), 
hash_function("Aliquam", 9000), 
hash_function("aliquet", 9000), 
hash_function("amet", 9000), 
hash_function("ante", 9000), 
hash_function("arcu", 9000), 
hash_function("at", 9000), 
hash_function("auctor", 9000), 
hash_function("augue", 9000), 
hash_function("bibendum", 9000), 
hash_function("blandit", 9000), 
hash_function("commodo", 9000), 
hash_function("condimentum", 9000), 
hash_function("congue", 9000), 
hash_function("consectetur", 9000), 
hash_function("consequat", 9000), 
hash_function("convallis", 9000), 
hash_function("Cras", 9000), 
hash_function("Curabitur", 9000), 
hash_function("cursus", 9000), 
hash_function("dapibus", 9000), 
hash_function("diam", 9000), 
hash_function("dictum", 9000), 
hash_function("dignissim", 9000), 
hash_function("dolor", 9000), 
hash_function("Donec", 9000), 
hash_function("dui", 9000), 
hash_function("Duis", 9000), 
hash_function("efficitur", 9000), 
hash_function("egestas", 9000), 
hash_function("eget", 9000), 
hash_function("eleifend", 9000), 
hash_function("elementum", 9000), 
hash_function("elit", 9000), 
hash_function("enim", 9000), 
hash_function("erat", 9000), 
hash_function("eros", 9000), 
hash_function("est", 9000), 
hash_function("et", 9000), 
hash_function("Etiam", 9000), 
hash_function("eu", 9000), 
hash_function("euismod", 9000), 
hash_function("ex", 9000), 
hash_function("facilisis", 9000), 
hash_function("fames", 9000), 
hash_function("faucibus", 9000), 
hash_function("felis", 9000), 
hash_function("fermentum", 9000), 
hash_function("feugiat", 9000), 
hash_function("finibus", 9000), 
hash_function("Fusce", 9000), 
hash_function("gravida", 9000), 
hash_function("habitant", 9000), 
hash_function("hendrerit", 9000), 
hash_function("iaculis", 9000), 
hash_function("id", 9000), 
hash_function("imperdiet", 9000), 
hash_function("in", 9000), 
hash_function("In", 9000), 
hash_function("Integer", 9000), 
hash_function("interdum", 9000), 
hash_function("ipsum", 9000), 
hash_function("justo", 9000), 
hash_function("lacinia", 9000), 
hash_function("lacus", 9000), 
hash_function("laoreet", 9000), 
hash_function("lectus", 9000), 
hash_function("leo", 9000), 
hash_function("libero", 9000), 
hash_function("ligula", 9000), 
hash_function("lobortis", 9000), 
hash_function("lorem", 9000), 
hash_function("Lorem", 9000), 
hash_function("luctus", 9000), 
hash_function("Maecenas", 9000), 
hash_function("magna", 9000), 
hash_function("malesuada", 9000), 
hash_function("massa", 9000), 
hash_function("mattis", 9000), 
hash_function("mauris", 9000), 
hash_function("Mauris", 9000), 
hash_function("maximus", 9000), 
hash_function("metus", 9000), 
hash_function("mi", 9000), 
hash_function("molestie", 9000), 
hash_function("mollis", 9000), 
hash_function("morbi", 9000), 
hash_function("Morbi", 9000), 
hash_function("Nam", 9000), 
hash_function("nec", 9000), 
hash_function("neque", 9000), 
hash_function("netus", 9000), 
hash_function("nibh", 9000), 
hash_function("nisi", 9000), 
hash_function("nisl", 9000), 
hash_function("non", 9000), 
hash_function("nulla", 9000), 
hash_function("Nulla", 9000), 
hash_function("Nullam", 9000), 
hash_function("nunc", 9000), 
hash_function("Nunc", 9000), 
hash_function("odio", 9000), 
hash_function("orci", 9000), 
hash_function("ornare", 9000), 
hash_function("pellentesque", 9000), 
hash_function("Pellentesque", 9000), 
hash_function("pharetra", 9000), 
hash_function("Phasellus", 9000), 
hash_function("placerat", 9000), 
hash_function("porta", 9000), 
hash_function("porttitor", 9000), 
hash_function("posuere", 9000), 
hash_function("Praesent", 9000), 
hash_function("pretium", 9000), 
hash_function("Proin", 9000), 
hash_function("pulvinar", 9000), 
hash_function("purus", 9000), 
hash_function("quam", 9000), 
hash_function("quis", 9000), 
hash_function("Quisque", 9000), 
hash_function("rhoncus", 9000), 
hash_function("risus", 9000), 
hash_function("rutrum", 9000), 
hash_function("sagittis", 9000), 
hash_function("sapien", 9000), 
hash_function("scelerisque", 9000), 
hash_function("sed", 9000), 
hash_function("Sed", 9000), 
hash_function("sem", 9000), 
hash_function("semper", 9000), 
hash_function("senectus", 9000), 
hash_function("sit", 9000), 
hash_function("sodales", 9000), 
hash_function("sollicitudin", 9000), 
hash_function("suscipit", 9000), 
hash_function("Suspendisse", 9000), 
hash_function("tellus", 9000), 
hash_function("tempor", 9000), 
hash_function("tempus", 9000), 
hash_function("tincidunt", 9000), 
hash_function("tortor", 9000), 
hash_function("tristique", 9000), 
hash_function("turpis", 9000), 
hash_function("ullamcorper", 9000), 
hash_function("ultrices", 9000), 
hash_function("ultricies", 9000), 
hash_function("urna", 9000), 
hash_function("ut", 9000), 
hash_function("Ut", 9000), 
hash_function("varius", 9000), 
hash_function("vehicula", 9000), 
hash_function("vel", 9000), 
hash_function("velit", 9000), 
hash_function("venenatis", 9000), 
hash_function("vestibulum", 9000), 
hash_function("Vestibulum", 9000), 
hash_function("vitae", 9000), 
hash_function("viverra", 9000), 
hash_function("volutpat", 9000), 
hash_function("vulputate", 9000)
};

  for (int i = 0; i < (int)(sizeof(is) / sizeof(is[0])); i++) {
    printf("%d -> %d\n", i, is[i]);
  }

  return 0;
}

int hash_function(char *key, size_t size) {
  unsigned long index = 5381;
  char *k = key;
  int c = *k;

  while(c) {
    index = ((index << 5) + index) + c;
    c = *k++;
  }

  return index % size;
}
