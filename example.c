int mid(int x, int y, int z){
    int ret = z;
    if (y < z) {
        if (x < y) {
            ret = y;
        } else if (x < z) {
            ret = (x + 0);

        }
    } else {
        if (x > y) {
            ret = y;
        } else if (x > z) {

        }
    }
    return ret;
}
