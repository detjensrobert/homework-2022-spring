---
header-includes:
  - \usepackage{helvet}
  - \renewcommand{\familydefault}{\sfdefault}
  - \usepackage{fancyhdr}
  - \pagestyle{fancy}
  - \rhead{Robert Detjens - detjensr}
  - \renewcommand{\headrulewidth}{0pt}
colorlinks: true
---

# Test & Inspection Report

## Project Information

### URL

<https://github.com/sous-chefs>, specifically <https://github.com/sous-chefs/selinux>

### License

Apache 2

### Description

The Sous Chefs cookbooks are a large family of over 180 open-source libraries
[Chef configuration management](https://chef.io) cookbooks and are heavily used
by the Chef community. These cookbooks are libraries written in Ruby and use
Chef resources to configure particular aspects of the system.

These cookbooks are aimed at sysadmins using Chef to manage their fleet of
machines. Chef provides a large number of basic tools as part of itself for
installing packages, creating files, etc; however there are very few
service-specific interactions. The Sous Chefs cookbooks provide these resources
via community-maintained cookbooks, which provide resources for e.g. configuring
an NGINX site, installing MySQL, and so forth.

## Report

The specific cookbook that I contributed testing to was the [`selinux`
cookbook](https://github.com/sous-chefs/selinux), as part of a larger
consolidation move between it and another SELinux-related cookbook
([`selinux-policy` cookbook](https://github.com/sous-chefs/selinux)). There were
a number of resources that were duplicated between them, and it makes more sense
for all the SELinux resources to be in one place.

As part of this merge, I added both unit and integration tests for the new
resources. The `selinux-policy` cookbook had unit tests for its resources, but
no integration tests. `selinux` had only a single unit test, but did have
integration tests. The existing tests were improved, and new tests were written
to achieve full coverage of all resources for both unit and integration tests.

Both unit tests and integration tests in the Chef ecosystem are done using a
flavor of the RSpec testing library: ChefSpec for unit, and InSpec for
integration. Both are similar to RSpec, but have added DSL functions to test
that Chef resources are run with expected parameters for a given recipe or to
more easily check system state on a configured system.

### Pull Request

The pull request for these changes is available here <https://github.com/sous-chefs/selinux/pull/79>.

Test files are located at:

  - `spec/unit/resources/`
  - `test/integration/$SUITE/controls/`

\pagebreak

One of the new unit tests: (`spec/unit/resources/boolean_spec.rb`)

```rb
require 'spec_helper'

describe 'selinux_boolean' do
  step_into :selinux_boolean
  platform 'centos'

  stubs_for_provider('selinux_boolean[test]') do |provider|
    allow(provider).to receive_shell_out('getenforce', stdout: 'Permissive')
  end

  context 'when boolean is set to incorrect value' do
    recipe do
      selinux_boolean 'test' do
        value 'on'
      end
    end

    stubs_for_resource('selinux_boolean[test]') do |resource|
      allow(resource).to receive_shell_out('getsebool test', stdout: 'off')
    end

    # this is what actually checks that the bool was set correctly
    # incorrect commands would not be stubbed and would throw error
    stubs_for_provider('selinux_boolean[test]') do |provider|
      allow(provider).to receive_shell_out('setsebool -P test on')
    end

    # needed to have the test run
    it { is_expected.to set_selinux_boolean('test') }
  end

  context 'when boolean is set to correct value' do
    recipe do
      selinux_boolean 'test' do
        value 'on'
      end
    end

    stubs_for_resource('selinux_boolean[test]') do |resource|
      allow(resource).to receive_shell_out('getsebool test', stdout: 'on')
    end

    # dont stub the set command since the resource should not actually
    # shell out for it since it is already set to the correct value
    # stubs_for_provider("selinux_boolean[test]") do |provider|
    #   allow(provider).to receive_shell_out('setsebool -P test on')
    # end

    # needed to have the test run
    it { is_expected.to set_selinux_boolean('test') }
  end
end
```
