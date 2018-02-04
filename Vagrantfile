###############################################################################
# (C) Copyright 2015,2017 Douglas B. Cuthbertson
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the MIT license. See LICENSE for details.
###############################################################################

# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

  # Start with "vagrant up linux"
  config.vm.define "linux" do |devlinux|
    devlinux.vm.box = "bento/ubuntu-16.04"
  end

  # Vagrant Configuration per https://forums.freebsd.org/threads/52717/
  # but slightly modified. I changed "--cpus" from "1" to "2"
  # Start with "vagrant up freebsd"
  #config.vm.define "freebsd" do |devbsd|
  #  devbsd.vm.guest = :freebsd
  #  # VirtualBox shared folders are not supported on FreeBSD
  #  # FreeBSD doesn't support SMB shared folders
  #  # NFS folders do not work on Windows hosts.
  #  # Need rsync installed on host OS (Windows in my case).
  #  #devbsd.vm.synced_folder ".", "/vagrant", type: "rsync", rsync__exclude: [".git/", ".vagrant/" ".vs/", ".vscode/"]
  #  devbsd.vm.synced_folder ".", "/vagrant", id: "vagrant-root", disabled: true
  #  devbsd.vm.box = "freebsd/FreeBSD-11.0-CURRENT"
  #  devbsd.ssh.shell = "sh"
  #  devbsd.vm.base_mac = "080027d14c66"
  #  devbsd.vm.provider :virtualbox do |vb|
  #      vb.customize ["modifyvm", :id, "--memory", 1024]
  #      vb.customize ["modifyvm", :id, "--cpus", "2"]
  #      vb.customize ["modifyvm", :id, "--hwvirtex", "on"]
  #      vb.customize ["modifyvm", :id, "--audio", "none"]
  #      vb.customize ["modifyvm", :id, "--nictype1", "virtio"]
  #      vb.customize ["modifyvm", :id, "--nictype2", "virtio"]
  #  end
  #end
end
