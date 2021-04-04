static void vol_write_header(VOLID_t id)
{
    protium_volid_t *vol = id;
    vol->flag &= (~VOL_FLAG_WRITE);
    if(pwrite(vol->fid, &vol->header, sizeof(vol_head_t), 0)
        != sizeof(vol_head_t)) {
        printf("%s: write header failed!\n", __func__);
        abort();
    }
}
/*
static void
safe_pread(int fd, void *buf, size_t count, off_t offset)
{
} */
